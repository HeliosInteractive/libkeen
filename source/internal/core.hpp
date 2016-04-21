#pragma once

#include "keen.hpp"
#include "asio.hpp"

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

private:
    explicit                    Core();
    static CoreRef              instance(AccessType);

private:
    asio::io_service            mIoService;
    std::shared_ptr<asio::io_service::work>
                                mServiceWork;
    std::vector<std::thread>    mThreadPool;
    std::vector<LoggerRef>      mLoggerRefs;
    CurlRef                     mCurlRef;
    CacheRef                    mCacheRef;
};

}}
