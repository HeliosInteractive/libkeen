#include "common.hpp"
#include "keen.h"

namespace {
struct ScopedClientKeenIO
{
    ScopedClientKeenIO() : instance(libkeen_client_keenio_make()) {};
    ~ScopedClientKeenIO() { libkeen_client_keenio_free(instance); }
    void* instance = nullptr;
};
}

SCENARIO("Shared library functionality", "[shared]")
{
    REQUIRE(libkeen_version_major() >= 0);
    REQUIRE(libkeen_version_minor() >= 0);
    REQUIRE(libkeen_version_patch() >= 0);

    GIVEN("An instance of Keen IO client")
    {
        ScopedClientKeenIO client;
        REQUIRE(client.instance != nullptr);

        THEN("Core instances are checked")
        {
            // 1 for client, 1 for internal and 1 for use_count call
            REQUIRE(libkeen_core_use_count() == 3);
        }
    }
}
