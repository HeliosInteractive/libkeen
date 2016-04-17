#pragma once

#include "asio.hpp"

#include <memory>
#include <vector>
#include <thread>
#include <string>

namespace libkeen {
namespace internal {

using CoreRef       = std::shared_ptr< class Core >;
using LoggerRef     = std::shared_ptr< class Logger >;
using LoggerRefs    = std::vector< LoggerRef >;

class Core
{
public:
    ~Core();
    void                        postEvent(const std::string& name, const std::string& data);
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
    LoggerRefs                  mLoggerRefs;
};

}}
