#pragma once

#include <string>

namespace libkeen {

class Config
{
public:
    const std::string&  getProjectId() const;
    void                setProjectId(const std::string& id);
    const std::string&  getWriteKey() const;
    void                setWriteKey(const std::string& key);

private:
    std::string         mProjectId;
    std::string         mWriteKey;
};

}
