#include "keen/version.hpp"

namespace libkeen {

int Version::getMajor()
{
    return MAJOR;
}

int Version::getMinor()
{
    return MINOR;
}

int Version::getPatch()
{
    return PATCH;
}

}
