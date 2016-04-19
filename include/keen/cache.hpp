#pragma once

#include "keen/fwd.hpp"

#include <memory>
#include <vector>
#include <string>

namespace libkeen {

class Cache
{
public:
    ~Cache();
    static CacheRef         ref();
    void                    push(const std::string& url, const std::string& data);
    bool                    exists(const std::string& url, const std::string& data) const;
    void                    pop(std::vector<std::pair<std::string, std::string>>& records, unsigned count) const;
    void                    remove(const std::string& url, const std::string& data);
    bool                    connected() const;
    void                    clear();

private:
    Cache();
    sqlite3*                mConnection;
    std::vector<LoggerRef>  mLoggerRefs;
};

}
