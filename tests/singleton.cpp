#include "common.hpp"
#include "core.hpp"

TEST_CASE("Singleton Check", "[singleton]")
{
    using namespace libkeen;
    using namespace libkeen::internal;

    // first time access, must have a use count of 0
    REQUIRE(Core::useCount() == 0);

    {
        // let's take a reference to core
        auto coreRef = Core::instance();
        
        // This must be 2, one for coreRef
        // and another one for internal
        REQUIRE(Core::useCount() == 2);
    }

    // use count should be one again here
    REQUIRE(Core::useCount() == 1);

    {
        // let's take a reference to core again
        auto coreRef1 = Core::instance();

        // This must be 2, one for coreRef
        // and another one for internal
        REQUIRE(Core::useCount() == 2);

        // release current instance
        Core::release();

        // current instance must be null
        REQUIRE(Core::useCount() == 0);

        // old instance must be valid only for coreRef1
        REQUIRE(coreRef1.use_count() == 1);

        // this creates a new internal instance
        auto coreRef2 = Core::instance();

        // This must be 2, one for coreRef
        // and another one for internal
        REQUIRE(Core::useCount() == 2);
    }

    // release current instance
    Core::release();

    // current instance must be null
    REQUIRE(Core::useCount() == 0);
}
