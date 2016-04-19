#include "common.hpp"
#include "keen.hpp"

TEST_CASE("Sanity Check", "[sanity]")
{
    REQUIRE(1 == 1);
    REQUIRE(libkeen::Version::getMajor() >= 0);
    REQUIRE(libkeen::Version::getMinor() >= 0);
    REQUIRE(libkeen::Version::getPatch() >= 0);
}
