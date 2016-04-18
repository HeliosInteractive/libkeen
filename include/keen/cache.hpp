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
    void                push(const std::string& name, const std::string& event);
    bool                exists(const std::string& name, const std::string& event) const;
    void                pop(std::vector<std::pair<std::string, std::string>>& records, unsigned count) const;
    void                remove(const std::string& name, const std::string& event);
    bool                connected() const;
    void                clear();

private:
    Cache();
    sqlite3             *mConnection;
};

}
