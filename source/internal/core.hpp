#pragma once

#include "keen.hpp"
#include "asio.hpp"

namespace libkeen {
namespace internal {

class Core
{
public:
    ~Core();
    void                        postEvent(Client& client, const std::string& name, const std::string& data);
    void                        postCache(unsigned count);
    static unsigned             useCount();
    static CoreRef              instance();
    static void                 release();

private:
    enum class AccessType       { Current, Renew, Release };
    static CoreRef              instance(AccessType);

private:
    Core();

private:
    asio::io_service            mIoService;
    asio::io_service::work      mWork;
    std::vector<std::thread>    mThreadPool;
    std::vector<LoggerRef>      mLoggerRefs;
    CurlRef                     mCurlRef;
    CacheRef                    mCacheRef;
};

}}
