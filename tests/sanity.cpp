#include "common.hpp"
#include "keen.hpp"

TEST_CASE("Sanity Check", "[sanity]")
{
    REQUIRE(1 == 1);
    REQUIRE(libkeen::version::getMajor() >= 0);
    REQUIRE(libkeen::version::getMinor() >= 0);
    REQUIRE(libkeen::version::getPatch() >= 0);
}
