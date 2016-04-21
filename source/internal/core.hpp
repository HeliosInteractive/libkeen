#pragma once

#include "keen.hpp"
#include "asio.hpp"

#include <functional>
#include <unordered_map>
#include <future>

namespace libkeen {
namespace internal {

class Core
{
public:
    virtual ~Core();
    void                        postEvent(const std::string& url, const std::string& json);
    void                        postCache(unsigned count);
    void                        flush();
    static unsigned             useCount();
    static CoreRef              instance();
    static void                 release();

private:
    enum class AccessType       { Current, Renew, Release };
    using Task                  = std::function<void()>;
    using TaskRef               = std::shared_ptr<Task>;

private:
    explicit                    Core();
    static CoreRef              instance(AccessType);

private:
    asio::io_service            mIoService;
    asio::io_service::work      mServiceWork;
    std::vector<std::thread>    mThreadPool;
    std::vector<LoggerRef>      mLoggerRefs;
    std::vector<TaskRef>        mPendingTasks;
    std::mutex                  mPendingLock;
    CurlRef                     mCurlRef;
    CacheRef                    mCacheRef;
};

}}
