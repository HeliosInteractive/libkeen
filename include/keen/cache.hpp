#pragma once

#include <memory>
#include <vector>
#include <string>

struct sqlite3;

namespace libkeen {
namespace internal {

using Sqlite3Ref = std::shared_ptr< class Cache >;

class Cache
{
public:
    ~Cache();
    static Sqlite3Ref   ref();
    void                push(const std::string& record);
    void                pop(const std::vector<std::string>& records, int count);
    void                remove(const std::string& record);

private:
    Cache();
    sqlite3             *mConnection;
};

}}
