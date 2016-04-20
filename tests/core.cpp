#include "common.hpp"

#include "keen/client.hpp"

#include "internal/curl.hpp"
#include "internal/core.hpp"
#include "internal/cache.hpp"

namespace {
void run_test_server() {
    int status = std::system("node server.js");
}}

namespace {
class CoreUnderTest : public libkeen::internal::Core
{
public:
    CoreUnderTest() {}

    enum class Condition { Pass, Fail, Clear, Count, Shutdown };

    virtual std::string buildAddress(const std::string& /*id*/, const std::string& /*key*/, const std::string& /*name*/) override
    {
        return buildAddress();
    }

    std::string buildAddress()
    {
        std::string url_pass        = "http://localhost:8080/pass";
        std::string url_fail        = "http://localhost:8080/fail";
        std::string url_clear       = "http://localhost:8080/clear";
        std::string url_count       = "http://localhost:8080/count";
        std::string url_shutdown    = "http://localhost:8080/shutdown";

        if (mCond == Condition::Pass) return url_pass;
        else if (mCond == Condition::Fail) return url_fail;
        else if (mCond == Condition::Clear) return url_clear;
        else if (mCond == Condition::Count) return url_count;
        else if (mCond == Condition::Shutdown) return url_shutdown;
        else return url_fail;
    }

    void setCond(Condition c) { mCond = c; }

private:
    Condition mCond = Condition::Pass;
};
}

TEST_CASE("Network Check", "[network]")
{
    using namespace libkeen;
    using namespace libkeen::internal;

    std::thread     server_thread(run_test_server);
    CoreUnderTest   core;
    Client          client;
    Cache           cache;
    Curl            curl;

    // just a sanity check
    core.setCond(CoreUnderTest::Condition::Pass);
    REQUIRE(curl.sendEvent(core.buildAddress(), ""));

    // Test a successful event
    std::string name{ "name" };
    std::string data{ "data" };
    core.postEvent(client, name, data);
    core.flush();
    REQUIRE(!cache.exists(core.buildAddress(), data));
    
    // Test a failed successful event
    core.setCond(CoreUnderTest::Condition::Fail);
    core.postEvent(client, name, data);
    core.flush();
    REQUIRE(cache.exists(core.buildAddress(), data));

    // Shutdown server
    core.setCond(CoreUnderTest::Condition::Shutdown);
    curl.sendEvent(core.buildAddress(), "");
    
    if (server_thread.joinable())
        server_thread.join();
}
