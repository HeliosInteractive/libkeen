#include "core.hpp"
#include "logger.hpp"
#include "keen/client.hpp"

#include "internal/curl.hpp"
#include "internal/cache.hpp"

namespace libkeen {
namespace internal {

CoreRef Core::instance(AccessType type)
{
    static CoreRef      core;
    static std::mutex   mutex;

    std::lock_guard<decltype(mutex)> lock(mutex);

    if (type == AccessType::Current)
    {
        /* no-op */
    }
    else if (type == AccessType::Release)
    {
        if (core) core.reset();
    }
    else if (type == AccessType::Renew)
    {
        if (!core) core.reset(new Core);
    }

    return core;
}

CoreRef Core::instance()
{
    if (!instance(AccessType::Current))
        return instance(AccessType::Renew);
    else
        return instance(AccessType::Current);
}

void Core::release()
{
    instance(AccessType::Release);
}

Core::Core()
    : mServiceWork(mIoService)
    , mCurlRef(std::make_shared<Curl>())
    , mCacheRef(std::make_shared<Cache>())
{
    Logger::pull(mLoggerRefs);
    flush();
}

Core::~Core()
{
    try
    {
        LOG_INFO("Stopping IO service");
        mIoService.stop();

        for (std::thread& thread : mThreadPool)
        {
            LOG_INFO("Shutting down thread " << thread.get_id());
            if (thread.joinable()) thread.join();
        }
    }
    catch (const std::exception& ex)
    {
        LOG_WARN("Unable to shutdown the core: " << ex.what());
        return;
    }
    catch (...)
    {
        LOG_WARN("Unable to shutdown the core. Giving up");
        return;
    }

    LOG_INFO("Core is shutdown.");
}

void Core::postEvent(const std::string& url, const std::string& json)
{
    try
    {
        LOG_DEBUG("Attempting to post and event to: " << url << " with json: " << json);

        TaskRef task{ std::make_shared<Task>([this, url, json]
        {
            if (!mCurlRef->sendEvent(url, json))
                mCacheRef->push(url, json);
        }) };

        {
            std::lock_guard<std::mutex> lock(mPendingLock);
            mPendingTasks.push_back(task);
        }

        mIoService.post([this, task]
        {
            if (task)
            {
                (*task)();
            }
            else
            {
                LOG_ERROR("Posted task is invalid");
            }
            
            std::lock_guard<std::mutex> lock(mPendingLock);
            mPendingTasks.erase(std::find(mPendingTasks.cbegin(), mPendingTasks.cend(), task));
        });
    }
    catch (const std::exception& ex) {
        LOG_ERROR("Core postEvent threw an exception: " << ex.what());
    } catch (...) {
        LOG_ERROR("Core postEvent threw an exception.");
    }
}

void Core::postCache(unsigned count)
{
    try
    {
        LOG_DEBUG("Attempting to post cache with count: " << count);

        mIoService.post([this, count]
        {
            std::vector<std::pair<std::string, std::string>> caches;
            mCacheRef->pop(caches, count);

            LOG_DEBUG("Cache entries trying to send out: " << caches.size());

            for (auto entry : caches)
            {
                LOG_DEBUG("Attempting to post and event to: " << entry.first << " with data: " << entry.second);

                mIoService.post([this, entry]
                {
                    if (mCurlRef->sendEvent(entry.first, entry.second))
                        mCacheRef->remove(entry.first, entry.second);
                });
            }
        });
    }
    catch (const std::exception& ex) {
        LOG_ERROR("Core postCache threw an exception: " << ex.what());
    } catch (...) {
        LOG_ERROR("Core postCache threw an exception.");
    }
}

void Core::flush()
{
    LOG_INFO("Stopping IO service");
    mIoService.stop();

    for (std::thread& thread : mThreadPool)
    {
        LOG_INFO("Shutting down thread " << thread.get_id());
        if (thread.joinable()) thread.join();
    }

    if (!mThreadPool.empty()) mThreadPool.clear();

    LOG_INFO("Executing pending tasks");
    {
        for (auto& task : mPendingTasks)
            (*task)();
    }
    mPendingTasks.clear();

    LOG_INFO("Resetting IO service");
    mIoService.reset();

    // hardware_concurrency can return zero, in that case one thread is forced
    unsigned num_threads = std::thread::hardware_concurrency();

    if (num_threads == 0)
    {
        LOG_WARN("hardware_concurrency returned 0. Forcing one thread.");
        num_threads = 1;
    }

    for (unsigned t = 0; t < num_threads; ++t)
    {
        mThreadPool.push_back(std::thread([this] { mIoService.run(); }));
        LOG_INFO("Spawned thread " << mThreadPool.back().get_id());
    }

    LOG_INFO("Thread pool size: " << mThreadPool.size());
}

unsigned Core::useCount()
{
    if (!instance(AccessType::Current)) return 0;
    // minus one to take current method's ref into account
    else return static_cast<unsigned>(instance(AccessType::Current).use_count()) - 1;
}

}}
