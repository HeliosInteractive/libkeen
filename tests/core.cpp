#include "common.hpp"

#include "keen/client.hpp"

#include "internal/curl.hpp"
#include "internal/core.hpp"
#include "internal/cache.hpp"
#include "internal/logger.hpp"

TEST_CASE("Network Check", "[network]")
{
    using namespace libkeen;
    using namespace libkeen::internal;

    TestServer          server;
    CoreRef             core = Core::instance();
    Cache               cache;
    Curl                curl;
    ScopedTestServer    sts(curl);

    // just sanity checks
    REQUIRE(core);
    REQUIRE(curl.sendEvent(server.address(TestServer::Condition::Pass), ""));

    // Test a successful event
    std::string data{ "data" };
    core->postEvent(server.address(TestServer::Condition::Pass), data);
    core->flush();
    REQUIRE(!cache.exists(server.address(TestServer::Condition::Pass), data));
    
    // Test a failed event
    core->postEvent(server.address(TestServer::Condition::Fail), data);
    core->flush();
    REQUIRE(cache.exists(server.address(TestServer::Condition::Fail), data));

    // Clear server
    curl.sendEvent(server.address(TestServer::Condition::Clear), "");

    // Test count of events sent
    auto count = 50;
    for (auto index = 0; index < count; ++index)
        core->postEvent(server.address(TestServer::Condition::Pass), data);
    core->flush();
    std::string reply;
    REQUIRE(curl.sendEvent(server.address(TestServer::Condition::Count), "", reply));
    REQUIRE(reply == std::to_string(count));

    // Test caching events
    cache.clear();
    for (auto index = 0; index < count; ++index)
        core->postEvent(server.address(TestServer::Condition::Fail), data + std::to_string(index));
    core->flush();
    REQUIRE(cache.count() == count);
}
