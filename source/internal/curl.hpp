#pragma once

#include "keen/fwd.hpp"

#include <memory>
#include <string>
#include <vector>

namespace libkeen {
namespace internal {

/*!
 * @class Curl
 * @brief Handles interfacing with cURL.
 * @note  Realistically only one instance of this class
 *        Exists at a time and Core owns it.
 */
class Curl
{
public:
    Curl();
    //! send a application/json request to given url using cURL
    bool    sendEvent(const std::string& url, const std::string& json);

private:
    std::shared_ptr< class LibCurlHandle >
            mLibCurlHandle;
    std::vector<LoggerRef>
            mLoggerRefs;
};

}}
