#include "keen/client.hpp"
#include "core.hpp"

namespace libkeen
{

Client::Client()
    : mCoreRef(internal::Core::instance())
{
    /* no-op */
}

const Config& Client::getConfig() const
{
    return mConfig;
}

void Client::setConfig(const Config& config)
{
    mConfig = config;
}

void Client::sendEvent(const std::string& name, const std::string& data)
{
    mCoreRef->postEvent(*this, name, data);
}

}
