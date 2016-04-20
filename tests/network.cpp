#include "common.hpp"
#include "keen.hpp"

#include "internal/curl.hpp"
#include "internal/core.hpp"

namespace {
void run_test_server() {
    int status = std::system("node server.js");
}}

TEST_CASE("Network Check", "[network]")
{
    using namespace libkeen;
    using namespace libkeen::internal;

    std::string url_pass = "http://localhost:8080/pass";
    std::string url_fail = "http://localhost:8080/fail";

    std::thread server_thread(run_test_server);
    CoreRef     core = Core::instance();
    Curl        curl;

    // One single event
    std::string json = "{ \"data\": \"bs\" }";
    REQUIRE(curl.sendEvent(url_pass, json));

    std::string reply;
    REQUIRE(curl.sendEvent(url_pass, json, reply));
    REQUIRE(reply == "OK");

    core->flush();
    curl.sendEvent("http://localhost:8080/shutdown", "");
    
    if (server_thread.joinable())
        server_thread.join();
}
