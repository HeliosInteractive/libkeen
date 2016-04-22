#include "common.hpp"

SCENARIO("Client functionality", "[client]")
{
    using namespace libkeen;
    using namespace libkeen::internal;

    TestServer          server;
    CoreRef             core = Core::instance();
    Curl                curl;
    Cache               cache;
    ScopedTestServer    sts(curl);

    GIVEN("An instance of Client class")
    {
        using namespace libkeen::internal;

        ClientTest  client;
        sts.clear();

        THEN("Properties are set.")
        {
            std::string id  = "id";
            std::string key = "key";

            client.setProjectId(id);
            client.setWriteKey(key);

            REQUIRE(client.getProjectId() == id);
            REQUIRE(client.getWriteKey() == key);
        }

        THEN("An event is sent successfully.")
        {
            REQUIRE(sts.count() == 0);

            client.sendEvent("pass", "data");
            core->flush();
            REQUIRE(sts.count() == 1);
        }

        THEN("An event is failed and cached.")
        {
            cache.clear();

            REQUIRE(sts.count() == 0);
            REQUIRE(cache.count() == 0);

            client.sendEvent("fail", "data");
            core->flush();
            REQUIRE(sts.count() == 0);
            REQUIRE(cache.count() == 1);
        }

        THEN("An event is failed, cached and sent.")
        {
            cache.clear();

            REQUIRE(sts.count() == 0);
            REQUIRE(cache.count() == 0);

            client.sendEvent("fail", "data");
            core->flush();
            REQUIRE(sts.count() == 0);
            REQUIRE(cache.count() == 1);

            sts.shortCircuit();
            core->postCache(1);
            core->flush();
            REQUIRE(sts.count() == 1);
            REQUIRE(cache.count() == 0);
        }
    }
}
