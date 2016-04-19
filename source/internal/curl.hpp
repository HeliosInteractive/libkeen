#pragma once

#include "keen/fwd.hpp"

#include <memory>
#include <string>
#include <vector>

namespace libkeen {
namespace internal {

class Curl
{
public:
    Curl();
    bool    sendEvent(const std::string& url, const std::string& json);

private:
    std::shared_ptr< class LibCurlHandle >
            mLibCurlHandle;
    std::vector<LoggerRef>
            mLoggerRefs;
};

}}
