#pragma once

#include "keen/config.hpp"
#include <memory>

namespace libkeen
{

namespace internal { class Core; }
using CoreRef = std::shared_ptr< internal::Core >;

class Client
{
public:
    Client();
    void    setConfig(const Config& config);
    void    sendEvent(const std::string& name, const std::string& data);

private:
    Config  mConfig;
    CoreRef mCoreRef;
};

}
