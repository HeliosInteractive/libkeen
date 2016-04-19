#pragma once

#include <memory>
#include <vector>
#include <string>

struct sqlite3;

namespace libkeen {

using Sqlite3Ref = std::shared_ptr< class Cache >;

class Cache
{
public:
    ~Cache();
    static Sqlite3Ref   ref();
    void                push(const std::string& url, const std::string& data);
    bool                exists(const std::string& url, const std::string& data) const;
    void                pop(std::vector<std::pair<std::string, std::string>>& records, unsigned count) const;
    void                remove(const std::string& url, const std::string& data);
    bool                connected() const;
    void                clear();

private:
    Cache();
    sqlite3             *mConnection;
};

}
