#include "keen/config.hpp"

namespace libkeen
{

const std::string& Config::getProjectId() const
{
    return mProjectId;
}

void Config::setProjectId(const std::string& id)
{
    if (id.empty()) return;
    mProjectId = id;
}

const std::string& Config::getWriteKey() const
{
    return mWriteKey;
}

void Config::setWriteKey(const std::string& key)
{
    if (key.empty()) return;
    mWriteKey = key;
}

}
