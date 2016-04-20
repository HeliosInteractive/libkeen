#include "common.hpp"

#include "keen/client.hpp"

#include "internal/curl.hpp"
#include "internal/core.hpp"
#include "internal/cache.hpp"

namespace {
void run_test_server() {
    int status = std::system("node server.js");
}}

TEST_CASE("Network Check", "[network]")
{
    using namespace libkeen;
    using namespace libkeen::internal;

    std::string url_pass        = "http://localhost:8080/pass";
    std::string url_fail        = "http://localhost:8080/fail";
    std::string url_clear       = "http://localhost:8080/clear";
    std::string url_count       = "http://localhost:8080/count";
    std::string url_shutdown    = "http://localhost:8080/shutdown";

    std::thread server_thread(run_test_server);
    Curl        curl;

    // One single event (bool overload)
    std::string json = "{ \"data\": \"bs\" }";
    REQUIRE(curl.sendEvent(url_pass, json));

    // One single event (string overload)
    std::string reply;
    REQUIRE(curl.sendEvent(url_pass, json, reply));
    REQUIRE(reply == "OK");

    // Single failed event (through client)
    REQUIRE(!curl.sendEvent(url_fail, json));

    // Check count
    REQUIRE(curl.sendEvent(url_count, "", reply));
    REQUIRE(reply == "2");

    // Clear server
    curl.sendEvent(url_clear, "");
    REQUIRE(curl.sendEvent(url_count, "", reply));
    REQUIRE(reply == "0");

    // Shutdown server
    curl.sendEvent(url_shutdown, "");
    
    if (server_thread.joinable())
        server_thread.join();
}
