#pragma once

#include "asio.hpp"

#include <atomic>
#include <memory>
#include <vector>
#include <thread>
#include <string>

namespace libkeen {

class Cache;
class Client;

namespace internal {

using CoreRef       = std::shared_ptr< class Core >;
using LoggerRef     = std::shared_ptr< class Logger >;
using LoggerRefs    = std::vector< LoggerRef >;
using LibCurlRef    = std::shared_ptr< class LibCurl >;
using Sqlite3Ref    = std::shared_ptr< Cache >;

class Core
{
public:
    ~Core();
    void                        postEvent(Client& client, const std::string& name, const std::string& data);
    void                        postEvent(const std::string& url, const std::string& data, const std::function<void()>& callback);
    static unsigned             useCount();
    static CoreRef              instance();
    static void                 release();

private:
    enum class AccessType       { Current, Renew, Release };
    static CoreRef              instance(AccessType);
    void                        cacheMain();

private:
    Core();

private:
    asio::io_service            mIoService;
    asio::io_service::work      mWork;
    std::vector<std::thread>    mThreadPool;
    LoggerRefs                  mLoggerRefs;
    LibCurlRef                  mLibCurlRef;
    Sqlite3Ref                  mSqlite3Ref;
    std::atomic<bool>           mQuitCache;
    std::thread                 mCacheThread;
    unsigned                    mCacheInterval;
    unsigned                    mCacheSweepCount;
};

}}
