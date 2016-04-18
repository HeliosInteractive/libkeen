#include "keen/cache.hpp"
#include "logger.hpp"
#include "scoped.hpp"
#include "sqlite3.h"

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

void Cache::push(const std::string& name, const std::string& event)
{
    if (!mConnection) return;
    
    sqlite3_stmt *stmt = nullptr;
    Scoped<sqlite3_stmt> scope_bound_stmt(stmt);

    std::stringstream ss;
    ss << "INSERT OR IGNORE INTO cache (name, event) VALUES ('" << name << "','" << event << "')";

    if (sqlite3_prepare_v2(mConnection, ss.str().c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        LOG_ERROR("Unable to prepare the statement.");
        return;
    }

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        LOG_ERROR("Unable to step the statement.");
        return;
    }
}

bool Cache::exists(const std::string& name, const std::string& event) const
{
    if (!mConnection) return false;

    sqlite3_stmt *stmt = nullptr;
    Scoped<sqlite3_stmt> scope_bound_stmt(stmt);

    std::stringstream ss;
    ss << "SELECT * FROM cache WHERE name='" << name << "' AND event='" << event << "' LIMIT 1";

    if (sqlite3_prepare_v2(mConnection, ss.str().c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        LOG_ERROR("Unable to prepare the statement.");
        return false;
    }

    if (sqlite3_step(stmt) == SQLITE_ROW)
        return true;
    else
        return false;
}

void Cache::pop(std::vector<std::pair<std::string, std::string>>& records, unsigned count) const
{
    if (!mConnection) return;
    if (!records.empty()) records.clear();

    sqlite3_stmt *stmt = nullptr;
    Scoped<sqlite3_stmt> scope_bound_stmt(stmt);

    std::stringstream ss;
    ss << "SELECT name, event FROM cache LIMIT " << count;

    if (sqlite3_prepare_v2(mConnection, ss.str().c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        LOG_ERROR("Unable to prepare the statement.");
        return;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        std::string name{ reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)) };
        std::string data{ reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)) };
        records.push_back(std::make_pair(name, data));
    }
}

void Cache::remove(const std::string& name, const std::string& event)
{
    if (!mConnection) return;

    sqlite3_stmt *stmt = nullptr;
    Scoped<sqlite3_stmt> scope_bound_stmt(stmt);

    std::stringstream ss;
    ss << "DELETE FROM cache WHERE name='" << name << "' AND event='" << event << "'";

    if (sqlite3_prepare_v2(mConnection, ss.str().c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        LOG_ERROR("Unable to prepare the statement.");
        return;
    }

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        LOG_ERROR("Unable to step the statement.");
        return;
    }
}

}}
