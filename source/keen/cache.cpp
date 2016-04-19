#include "keen/cache.hpp"
#include "logger.hpp"
#include "scoped.hpp"
#include "sqlite3.h"

namespace libkeen {

Cache::Cache()
    : mConnection(nullptr)
{
    // Make sure logger stays alive for our life-time
    internal::Logger::pull(mLoggerRefs);

    if (sqlite3_threadsafe() != SQLITE_CONFIG_SERIALIZED)
    {
        LOG_WARN("Sqlite3 engine is not configured as serilized! This causes race conditions");
        mConnection = nullptr;
        return;
    }

    // Open a connection to cache database
    if (sqlite3_open("libkeen.db", &mConnection) != SQLITE_OK)
    {
        LOG_ERROR("Establishing database connection failed.");
        mConnection = nullptr;
        return;
    }
    else
    {
        LOG_INFO("Connection to cache database successfully established.");
        
        if (connected())
        {
            // Let's create the cache table if it does not exist
            std::string create_table_query = "CREATE TABLE IF NOT EXISTS cache (id INTEGER PRIMARY KEY, url VARCHAR(1024), data VARCHAR(4096), UNIQUE(url, data))";

            if (sqlite3_exec(mConnection, create_table_query.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK)
            {
                LOG_ERROR("Unable to create the cache table.");
                mConnection = nullptr;
                return;
            }
            else
            {
                LOG_INFO("Table query executed successfully.");
            }
        }
        else
        {
            LOG_ERROR("Connection to database does not exist.");
            mConnection = nullptr;
            return;
        }
    }
}

Cache::~Cache()
{
    if (connected() && sqlite3_close_v2(mConnection) == SQLITE_OK) {
        LOG_INFO("Cache database connection closed successfully.");
    } else {
        LOG_WARN("Unable to close database connection. Giving up.");
    }

    mConnection = nullptr;
}

void Cache::push(const std::string& url, const std::string& data)
{
    LOG_INFO("Pushing cache entry with url: " << url << " and data: " << data);

    if (!connected())
    {
        LOG_ERROR("A connection does not exist for this database operation.");
        return;
    }
    
    sqlite3_stmt *stmt = nullptr;
    internal::Scoped<sqlite3_stmt> scope_bound_stmt(stmt);

    std::stringstream ss;
    ss << "INSERT OR IGNORE INTO cache (url, data) VALUES ('" << url << "','" << data << "')";

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

    LOG_INFO("Cache entry with url: " << url << " and data: " << data << " pushed.");
}

bool Cache::exists(const std::string& url, const std::string& data) const
{
    if (!connected()) return false;

    sqlite3_stmt *stmt = nullptr;
    internal::Scoped<sqlite3_stmt> scope_bound_stmt(stmt);

    std::stringstream ss;
    ss << "SELECT * FROM cache WHERE url='" << url << "' AND data='" << data << "' LIMIT 1";

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
    if (!connected()) return;
    if (!records.empty()) records.clear();

    sqlite3_stmt *stmt = nullptr;
    internal::Scoped<sqlite3_stmt> scope_bound_stmt(stmt);

    std::stringstream ss;
    ss << "SELECT url, data FROM cache LIMIT " << count;

    if (sqlite3_prepare_v2(mConnection, ss.str().c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        LOG_ERROR("Unable to prepare the statement.");
        return;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        std::string url{ reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)) };
        std::string data{ reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)) };
        records.push_back(std::make_pair(url, data));
    }
}

void Cache::remove(const std::string& url, const std::string& data)
{
    LOG_INFO("Removing cache entry with url: " << url << " and data: " << data);

    if (!connected())
    {
        LOG_ERROR("A connection does not exist for this database operation.");
        return;
    }

    sqlite3_stmt *stmt = nullptr;
    internal::Scoped<sqlite3_stmt> scope_bound_stmt(stmt);

    std::stringstream ss;
    ss << "DELETE FROM cache WHERE url='" << url << "' AND data='" << data << "'";

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

    LOG_INFO("Cache entry with url: " << url << " and data: " << data << " removed.");
}

bool Cache::connected() const
{
    return mConnection != nullptr;
}

void Cache::clear()
{
    if (!connected()) return;

    sqlite3_stmt *stmt = nullptr;
    internal::Scoped<sqlite3_stmt> scope_bound_stmt(stmt);

    std::stringstream ss;
    ss << "DELETE FROM cache";

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

}
