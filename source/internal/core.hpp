#pragma once

#include "keen.hpp"
#include "asio.hpp"

namespace libkeen {
namespace internal {

class Core
{
public:
    virtual ~Core();
    void                        postEvent(Client& client, const std::string& name, const std::string& data);
    void                        postCache(unsigned count);
    void                        flush();
    static unsigned             useCount();
    static CoreRef              instance();
    static void                 release();

protected:
    Core();
    virtual std::string         buildAddress(const std::string& id, const std::string& key, const std::string& name);

private:
    enum class AccessType       { Current, Renew, Release };
    static CoreRef              instance(AccessType);

private:
    asio::io_service            mIoService;
    asio::io_service::work      mWork;
    std::vector<std::thread>    mThreadPool;
    std::vector<LoggerRef>      mLoggerRefs;
    CurlRef                     mCurlRef;
    CacheRef                    mCacheRef;
};

}}
