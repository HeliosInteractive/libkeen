#include "core.hpp"
#include "curl.hpp"
#include "logger.hpp"

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
    , mLibCurlRef(LibCurl::ref())
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

void Core::postEvent(const std::string& name, const std::string& data)
{

}

unsigned Core::useCount()
{
    if (!instance(AccessType::Current)) return 0;
    // minus one to take current method's ref into account
    else return static_cast<unsigned>(instance(AccessType::Current).use_count()) - 1;
}

}}
