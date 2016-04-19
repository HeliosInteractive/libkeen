#pragma once

#include "keen/cache.hpp"
#include "keen/fwd.hpp"
#include "curl.hpp"

#include "asio.hpp"

#include <atomic>
#include <vector>
#include <thread>
#include <string>

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
    Curl                        mLibCurl;
    Cache                       mSqlite3;
};

}}
