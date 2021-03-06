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
    : mServiceWork(new asio::io_service::work(mIoService))
    , mCurlRef(std::make_shared<Curl>())
    , mCacheRef(std::make_shared<Cache>())
{
    Logger::pull(mLoggerRefs);
    respawn();
}

void Core::respawn()
{
    LOG_INFO("Resetting IO service.");
    mIoService.reset();

    LOG_INFO("Allocating new work.");
    mServiceWork.reset(new asio::io_service::work(mIoService));

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

void Core::shutdown()
{
    if (!mIoService.stopped())
    {
        LOG_INFO("Clearing work.");
        if (mServiceWork) mServiceWork.reset();

        LOG_INFO("Waiting for pending handlers.");
        mIoService.run();

        LOG_INFO("Stopping IO service.");
        mIoService.stop();
    }

    if (!mThreadPool.empty())
    {
        for (auto& thread : mThreadPool)
        {
            LOG_INFO("Shutting down thread " << thread.get_id());
            if (thread.joinable()) thread.join();
        }

        mThreadPool.clear();
        LOG_INFO("Thread pool is empty.");
    }
}

Core::~Core()
{
    shutdown();
    LOG_INFO("Core is shutdown.");
}

void Core::postEvent(const std::string& url, const std::string& json)
{
    LOG_INFO("Attempting to post and event to: " << url << " with json: " << json);
    mIoService.post([=]
    {
        if (!mCurlRef->sendEvent(url, json))
            mCacheRef->push(url, json);
    });
}

void Core::postCache(unsigned count)
{
    LOG_INFO("Attempting to post cache with count: " << count);
    mIoService.post([this, count]
    {
        std::vector<std::pair<std::string, std::string>> caches;
        mCacheRef->pop(caches, count);

        for (auto& entry : caches)
        {
            LOG_DEBUG("Attempting to post cached event to: " << entry.first << " with json: " << entry.second);
            mIoService.post([this, entry]
            {
                if (mCurlRef->sendEvent(entry.first, entry.second))
                    mCacheRef->remove(entry.first, entry.second);
            });
        }
    });
}

void Core::flush()
{
    shutdown();
    respawn();
}

void Core::enableLogToFile(bool on /*= true*/)
{
    for (auto logger : mLoggerRefs)
        if (logger) logger->enableLogToFile(on);
}

void Core::enableLogToConsole(bool on /*= true*/)
{
    for (auto logger : mLoggerRefs)
        if (logger) logger->enableLogToConsole(on);
}

void Core::clearCache()
{
    if (mCacheRef)
        mCacheRef->clear();
}

unsigned Core::useCount()
{
    if (!instance(AccessType::Current)) return 0;
    // minus one to take current method's ref into account
    else return static_cast<unsigned>(instance(AccessType::Current).use_count()) - 1;
}

}}
