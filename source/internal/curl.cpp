#include <atomic>

#include "curl.hpp"
#include "scoped.hpp"
#include "logger.hpp"

#include "curl/curl.h"

namespace {
static std::atomic<bool> CURL_READY{ false };
}

namespace libkeen {
namespace internal {

CurlRef Curl::ref()
{
    static CurlRef instance{ new Curl };
    return instance;
}

bool Curl::sendEvent(const std::string& url, const std::string& json)
{
    if (!CURL_READY)
    {
        LOG_WARN("cURL is not ready. Invalid operation.");
        return false;
    }

    bool success = false;
    LOG_INFO("cURL is about to send an event to: " << url << " with json: " << json);

    if (auto curl = curl_easy_init())
    {
        curl_slist *headers = nullptr;

        Scoped<CURL>        scope_bound_curl(curl);
        Scoped<curl_slist>  scope_bound_slist(headers);

        headers = curl_slist_append(headers, "Accept: application/json");
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "charsets: utf-8");

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        auto res = curl_easy_perform(curl);

        if (res == CURLE_OK)
        {
            long http_code = 0;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

            if (http_code == 200 || http_code == 201)
                success = true;

            // FROM https://keen.io/docs/api/#errors
            // Check the response body for individual event statuses. Some or all may have failed.
            // TODO ^^^
        }
    }

    if (success) {
        LOG_INFO("cURL succesfully sent an event.");
    } else {
        LOG_ERROR("cURL failed to send an event.");
    }

    return success;
}

Curl::Curl()
{
    if (!CURL_READY && curl_global_init(CURL_GLOBAL_DEFAULT) == CURLE_OK)
    {
        CURL_READY = true;
        LOG_INFO("cURL is initialized successfully.");
    }
}

Curl::~Curl()
{
    if (CURL_READY)
    {
        curl_global_cleanup();
        LOG_INFO("cURL is shutdown.");
    }
    else
    {
        LOG_ERROR("cURL is unable to shutdown.");
    }
}

}}
