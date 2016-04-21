#pragma once

#include "keen.hpp"
#include "asio.hpp"

#include <functional>
#include <unordered_map>
#include <future>

namespace libkeen {
namespace internal {

using Task      = std::function<void()>;
using TaskRef   = std::shared_ptr<Task>;
using TaskVec   = std::vector<TaskRef>;

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
    TaskVec                     mTaskVec;
    std::mutex                  mTaskLock;
    CurlRef                     mCurlRef;
    CacheRef                    mCacheRef;
};

}}
