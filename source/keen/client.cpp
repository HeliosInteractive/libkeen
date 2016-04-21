#include "keen/client.hpp"
#include "internal/core.hpp"

namespace libkeen
{

Client::Client()
    : mCoreRef(internal::Core::instance())
    , mProjectId("invalid")
    , mWriteKey("invalid")
{
    /* no-op */
}

void Client::sendEvent(const std::string& name, const std::string& data)
{
    onSendEvent(name, data);
}

void Client::setProjectId(const std::string& id)
{
    if (id.empty()) return;

    std::lock_guard<decltype(mClientLock)> lock(mClientLock);
    mProjectId = id;
}

void Client::setWriteKey(const std::string& key)
{
    if (key.empty()) return;

    std::lock_guard<decltype(mClientLock)> lock(mClientLock);
    mWriteKey = key;
}

std::string Client::getProjectId() const
{
    std::lock_guard<decltype(mClientLock)> lock(mClientLock);
    return mProjectId;
}

std::string Client::getWriteKey() const
{
    std::lock_guard<decltype(mClientLock)> lock(mClientLock);
    return mWriteKey;
}

void Client::onSendEvent(const std::string& name, const std::string& data)
{
    std::lock_guard<decltype(mClientLock)> lock(mClientLock);
    
    std::stringstream ss;
    ss << "https://api.keen.io/3.0/projects/"
        << mProjectId
        << "/events/"
        << name
        << "?api_key="
        << mWriteKey;

    mCoreRef->postEvent(ss.str(), data);
}

}
