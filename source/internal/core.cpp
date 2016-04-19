#include "core.hpp"
#include "curl.hpp"
#include "logger.hpp"
#include "keen/cache.hpp"
#include "keen/client.hpp"

#include <mutex>

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
    : mWork(mIoService)
    , mLibCurlRef(Curl::ref())
    , mSqlite3Ref(Cache::ref())
{
    Logger::pull(mLoggerRefs);

    unsigned num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0)
    {
        LOG_WARN("hardware_concurrency returned 0. Forcing one thread.");
        num_threads = 1;
    }

    for (unsigned t = 0; t < num_threads; ++t)
    {
        mThreadPool.push_back(std::thread([this]{ mIoService.run(); }));
        LOG_INFO("Spawned thread " << mThreadPool.back().get_id());
    }

    LOG_INFO("Thread pool size: " << mThreadPool.size());
    LOG_INFO("Starting cache service.");
}

Core::~Core()
{
    LOG_INFO("Stopping IO service");
    mIoService.stop();

    for (std::thread& thread : mThreadPool)
    {
        LOG_INFO("Shutting down thread " << thread.get_id());
        thread.join();
    }
}

void Core::postEvent(Client& client, const std::string& name, const std::string& data)
{
    std::stringstream ss;
    ss  << "https://api.keen.io/3.0/projects/"
        << client.getConfig().getProjectId()
        << "/events/"
        << name
        << "?api_key="
        << client.getConfig().getWriteKey();
    std::string url{ ss.str() };
    
    mIoService.post([this, url, data]
    {
        if (!mLibCurlRef->sendEvent(url, data))
            mSqlite3Ref->push(url, data);
    });
}

void Core::postCache(unsigned count)
{
    mIoService.post([this, count]
    {
        std::vector<std::pair<std::string, std::string>> caches;
        mSqlite3Ref->pop(caches, count);

        for (auto entry : caches)
        {
            mIoService.post([this, entry]
            {
                if (mLibCurlRef->sendEvent(entry.first, entry.second))
                    mSqlite3Ref->remove(entry.first, entry.second);
            });
        }
    });
}

unsigned Core::useCount()
{
    if (!instance(AccessType::Current)) return 0;
    // minus one to take current method's ref into account
    else return static_cast<unsigned>(instance(AccessType::Current).use_count()) - 1;
}

}}
