#pragma once

#include "keen.hpp"

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
    //! send a application/json request to given url using cURL and returns the result body
    bool    sendEvent(const std::string& url, const std::string& json, std::string& reply);

private:
    std::shared_ptr< class LibCurlHandle >
            mLibCurlHandle;
    std::vector<LoggerRef>
            mLoggerRefs;
};

}}
