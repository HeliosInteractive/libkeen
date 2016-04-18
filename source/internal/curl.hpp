#pragma once

#include <memory>
#include <string>

namespace libkeen {
namespace internal {

using LibCurlRef = std::shared_ptr< class LibCurl >;

class LibCurl
{
public:
    ~LibCurl();
    static LibCurlRef   ref();
    bool                sendEvent(const std::string& url, const std::string& json);

private:
    LibCurl();
};

}}
