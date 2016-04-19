#pragma once

#include "keen/fwd.hpp"
#include "keen/config.hpp"

namespace libkeen
{

class Client
{
public:
    Client();
    const Config&   getConfig() const;
    void            setConfig(const Config& config);
    void            sendEvent(const std::string& name, const std::string& data);

private:
    Config          mConfig;
    CoreRef         mCoreRef;
};

}
