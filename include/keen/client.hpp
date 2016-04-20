#pragma once

#include "keen.hpp"

namespace libkeen
{

class Client
{
public:
    Client();
    void                sendEvent(const std::string& name, const std::string& data);
    void                setProjectId(const std::string& id);
    void                setWriteKey(const std::string& key);
    std::string         getProjectId() const;
    std::string         getWriteKey() const;

private:
    CoreRef             mCoreRef;
    mutable std::mutex  mClientLock;
    std::string         mProjectId;
    std::string         mWriteKey;
};

}
