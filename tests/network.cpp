#include "common.hpp"

#include "keen/client.hpp"

#include "internal/curl.hpp"
#include "internal/core.hpp"
#include "internal/cache.hpp"

TEST_CASE("Network Check", "[network]")
{
    using namespace libkeen;
    using namespace libkeen::internal;

    TestServer          server;
    Curl                curl;
    ScopedTestServer    sts(curl);

    // One single event (bool overload)
    std::string json = "{ \"data\": \"bs\" }";
    REQUIRE(curl.sendEvent(server.address(TestServer::Condition::Pass), json));

    // One single event (string overload)
    std::string reply;
    REQUIRE(curl.sendEvent(server.address(TestServer::Condition::Pass), json, reply));
    REQUIRE(reply == "OK");

    // Single failed event (through client)
    REQUIRE(!curl.sendEvent(server.address(TestServer::Condition::Fail), json));

    // Check count
    REQUIRE(curl.sendEvent(server.address(TestServer::Condition::Count), "", reply));
    REQUIRE(reply == "2");

    // Clear server
    curl.sendEvent(server.address(TestServer::Condition::Clear), "");
    REQUIRE(curl.sendEvent(server.address(TestServer::Condition::Count), "", reply));
    REQUIRE(reply == "0");
}
