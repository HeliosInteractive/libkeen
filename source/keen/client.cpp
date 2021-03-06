#include "keen/client.hpp"

#include "internal/core.hpp"
#include "internal/logger.hpp"

namespace libkeen
{

Client::Client()
    : mCoreRef(internal::Core::instance())
{
    LOG_DEBUG("Client instance is initialized.");
}

Client::~Client()
{
    LOG_DEBUG("Client instance is destructed.");
}

void Client::sendEvent(const std::string& collection, const std::string& json)
{
    if (!mCoreRef)
    {
        LOG_ERROR("Client does not have a valid Core handle.");
        return;
    }

    mCoreRef->postEvent(getEndpoint(collection), json);
}

ClientKeenIO::ClientKeenIO()
    : mProjectId("invalid")
    , mWriteKey("invalid")
{
    /* no-op */
}

void ClientKeenIO::setProjectId(const std::string& id)
{
    if (id.empty())
    {
        LOG_WARN("Empty project ID is supplied.");
        return;
    }

    std::lock_guard<decltype(mClientLock)> lock(mClientLock);
    mProjectId = id;
}

void ClientKeenIO::setWriteKey(const std::string& key)
{
    if (key.empty())
    {
        LOG_WARN("Empty write key is supplied.");
        return;
    }

    std::lock_guard<decltype(mClientLock)> lock(mClientLock);
    mWriteKey = key;
}

std::string ClientKeenIO::getProjectId() const
{
    std::lock_guard<decltype(mClientLock)> lock(mClientLock);
    return mProjectId;
}

std::string ClientKeenIO::getWriteKey() const
{
    std::lock_guard<decltype(mClientLock)> lock(mClientLock);
    return mWriteKey;
}

std::string ClientKeenIO::getEndpoint(const std::string& collection) const
{
    std::lock_guard<decltype(mClientLock)> lock(mClientLock);

    std::stringstream ss;
    ss << "https://api.keen.io/3.0/projects/"
        << mProjectId
        << "/events/"
        << collection
        << "?api_key="
        << mWriteKey;

    return ss.str();
}

}
