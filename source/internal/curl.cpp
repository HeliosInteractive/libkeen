#include "internal/curl.hpp"

#include "scoped.hpp"
#include "logger.hpp"

#include "curl/curl.h"

namespace libkeen {
namespace internal {

class LibCurlHandle
{
public:
    static std::shared_ptr< LibCurlHandle > ref() {
        static std::shared_ptr< LibCurlHandle > instance{ new LibCurlHandle };
        return instance;
    }

    LibCurlHandle();
    ~LibCurlHandle();
    bool isReady() const;

private:
    bool                    mReady = false;
    std::vector<LoggerRef>  mLoggerRefs;
};

LibCurlHandle::LibCurlHandle()
{
    LOG_INFO("Starting up cURL");
    internal::Logger::pull(mLoggerRefs);
    mReady = (curl_global_init(CURL_GLOBAL_DEFAULT) == CURLE_OK);
}

LibCurlHandle::~LibCurlHandle()
{
    LOG_INFO("Shutting down cURL");
    if (isReady()) { curl_global_cleanup(); }
}

bool LibCurlHandle::isReady() const
{
    return mReady;
}

bool Curl::sendEvent(const std::string& url, const std::string& json)
{
    if (!mLibCurlHandle || !mLibCurlHandle->isReady())
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

namespace {
static size_t write_cb(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}}

bool Curl::sendEvent(const std::string& url, const std::string& json, std::string& reply)
{
    if (!mLibCurlHandle || !mLibCurlHandle->isReady())
    {
        LOG_WARN("cURL is not ready. Invalid operation.");
        return false;
    }

    bool success = false;
    if (!reply.empty()) reply.clear();

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
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &reply);

        auto res = curl_easy_perform(curl);

        if (res == CURLE_OK)
        {
            long http_code = 0;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

            if (http_code == 200 || http_code == 201)
                success = true;
        }
    }

    if (success) {
        LOG_INFO("cURL succesfully sent an event.");
    }
    else {
        LOG_ERROR("cURL failed to send an event.");
    }

    return success;
}

Curl::Curl()
    : mLibCurlHandle(LibCurlHandle::ref())
{
    internal::Logger::pull(mLoggerRefs);

    if (mLibCurlHandle && mLibCurlHandle->isReady())
        LOG_INFO("cURL is initialized successfully.");
}

}}
