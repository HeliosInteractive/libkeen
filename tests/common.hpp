#pragma once

#ifndef CATCH_CONFIG_MAIN
#    define CATCH_CONFIG_MAIN
#endif

#include "catch.hpp"

#include "keen.hpp"
#include "keen/client.hpp"

#include "internal/core.hpp"
#include "internal/curl.hpp"
#include "internal/cache.hpp"

#include <string>
#include <thread>

namespace {
class TestServer
{
public:
    TestServer();
    ~TestServer();
    enum class          Condition { Pass, Fail, Clear, Count, Shutdown, ShortCircuit };
    static void         run();
    static std::string  address(Condition cond);

private:
    std::thread mThread;
};

TestServer::TestServer()
    : mThread(TestServer::run)
{}

TestServer::~TestServer()
{
    if (mThread.joinable())
        mThread.join();
}

void TestServer::run()
{
    int status = std::system("node server.js");
}

std::string TestServer::address(Condition cond)
{
    std::string url_pass        = "http://localhost:8080/pass";
    std::string url_fail        = "http://localhost:8080/fail";
    std::string url_clear       = "http://localhost:8080/clear";
    std::string url_count       = "http://localhost:8080/count";
    std::string url_shutdown    = "http://localhost:8080/shutdown";
    std::string url_sc          = "http://localhost:8080/short_circuit";

    if (cond == Condition::Pass) return url_pass;
    else if (cond == Condition::Fail) return url_fail;
    else if (cond == Condition::Clear) return url_clear;
    else if (cond == Condition::Count) return url_count;
    else if (cond == Condition::Shutdown) return url_shutdown;
    else if (cond == Condition::ShortCircuit) return url_sc;
    else return url_fail;
}

struct ScopedTestServer
{
    ScopedTestServer(libkeen::internal::Curl& curl) : mCurl(curl) {};
    ~ScopedTestServer() { mCurl.sendEvent(TestServer::address(TestServer::Condition::Shutdown), ""); };
    int count()
    {
        std::string reply;
        mCurl.sendEvent(TestServer::address(TestServer::Condition::Count), "", reply);
        return std::stoi(reply);
    }
    void clear()
    {
        mCurl.sendEvent(TestServer::address(TestServer::Condition::Clear), "");
    }
    void shortCircuit()
    {
        mCurl.sendEvent(TestServer::address(TestServer::Condition::ShortCircuit), "");
    }

private:
    libkeen::internal::Curl& mCurl;
};

class ClientTest : public libkeen::Client
{
    virtual std::string getEndpoint(const std::string& collection) const override
    {
        return "http://localhost:8080/" + collection;
    }
};

}
