#include "keen/config.hpp"

namespace
{
const static std::string EMPTY_STRING{"unassigned"};
}

namespace libkeen
{

const std::string& Config::getProjectId() const
{
    if (mProjectId.empty()) return EMPTY_STRING;
    else return mProjectId;
}

void Config::setProjectId(const std::string& id)
{
    if (id.empty()) return;
    mProjectId = id;
}

const std::string& Config::getWriteKey() const
{
    if (mWriteKey.empty()) return EMPTY_STRING;
    else return mWriteKey;
}

void Config::setWriteKey(const std::string& key)
{
    if (key.empty()) return;
    mWriteKey = key;
}

}
