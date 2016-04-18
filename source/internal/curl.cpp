#include "curl.hpp"
#include <curl/curl.h>

namespace {
struct ScopeReleaseCurlPointer
{
    ScopeReleaseCurlPointer(CURL *ptr) : mPointer(ptr) {};
    ~ScopeReleaseCurlPointer() { if (mPointer) curl_easy_cleanup(mPointer); };
    CURL *mPointer = nullptr;
};
struct ScopeReleaseCurlSlist
{
    ScopeReleaseCurlSlist(curl_slist *ptr) : mPointer(ptr) {};
    ~ScopeReleaseCurlSlist() { if (mPointer) curl_slist_free_all(mPointer); };
    curl_slist *mPointer = nullptr;
};
}

namespace libkeen {
namespace internal {

LibCurlRef LibCurl::ref()
{
    static LibCurlRef instance{ new LibCurl };
    return instance;
}

bool LibCurl::sendEvent(const std::string& url, const std::string& json)
{
    bool success = false;

    if (auto curl = curl_easy_init())
    {
        curl_slist *headers = nullptr;

        ScopeReleaseCurlPointer scoped_curl(curl);
        ScopeReleaseCurlSlist   scoped_slist(headers);

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

    return success;
}

LibCurl::LibCurl()
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

LibCurl::~LibCurl()
{
    curl_global_cleanup();
}

}}
