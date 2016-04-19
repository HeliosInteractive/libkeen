#pragma once

#include "keen/fwd.hpp"
#include <string>

namespace libkeen {
namespace internal {

class Curl
{
public:
    ~Curl();
    static CurlRef  ref();
    bool            sendEvent(const std::string& url, const std::string& json);

private:
    Curl();
};

}}
