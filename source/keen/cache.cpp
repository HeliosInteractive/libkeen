#include "keen/cache.hpp"
#include "logger.hpp"
#include <sqlite3.h>

namespace libkeen {
namespace internal {

Cache::Cache()
    : mConnection(nullptr)
{
    if (sqlite3_open("libkeen.db", &mConnection) != SQLITE_OK)
    {
        LOG_ERROR("Establishing database connection failed.");
        return;
    }
    
    if (mConnection)
    {
        std::string create_table_query = "CREATE TABLE IF NOT EXISTS cache (id INTEGER PRIMARY KEY, name VARCHAR(256), event VARCHAR(4096), UNIQUE(name, event))";
        if (sqlite3_exec(mConnection, create_table_query.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK)
            LOG_ERROR("Unable to create the cache table.");
    }
    else
        LOG_ERROR("Connection to database does not exist.");
}

Sqlite3Ref Cache::ref()
{
    static Sqlite3Ref instance{ new Cache };
    return instance;
}

Cache::~Cache()
{
    if (mConnection) sqlite3_close(mConnection);
    mConnection = nullptr;
}

void Cache::push(const std::string& record)
{
    if (!mConnection) return;
}

void Cache::pop(const std::vector<std::string>& records, int count)
{
    if (!mConnection) return;
}

void Cache::remove(const std::string& record)
{
    if (!mConnection) return;
}

}}
