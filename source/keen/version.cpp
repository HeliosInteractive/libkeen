#include "keen/version.hpp"

namespace libkeen {

int version::getMajor()
{
    return MAJOR;
}

int version::getMinor()
{
    return MINOR;
}

int version::getPatch()
{
    return PATCH;
}

}
