#pragma once

#include "keen.hpp"

namespace libkeen {
namespace internal {

class Cache
{
public:
    Cache();
    ~Cache();
    void                    push(const std::string& url, const std::string& data);
    bool                    exists(const std::string& url, const std::string& data) const;
    void                    pop(std::vector<std::pair<std::string, std::string>>& records, unsigned count) const;
    void                    remove(const std::string& url, const std::string& data);
    bool                    connected() const;
    void                    clear();
    int                     count();

private:
    sqlite3*                mConnection;
    std::vector<LoggerRef>  mLoggerRefs;
};

}}
