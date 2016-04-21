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

protected:
    virtual void        onSendEvent(const std::string& name, const std::string& data);
    CoreRef             mCoreRef;

private:
    mutable std::mutex  mClientLock;
    std::string         mProjectId;
    std::string         mWriteKey;
};

}
