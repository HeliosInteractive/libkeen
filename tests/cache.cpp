#include "common.hpp"
#include "asio.hpp"

#include "internal/cache.hpp"

SCENARIO("Cache functionality", "[cache]")
{
    std::vector<std::pair<std::string, std::string>> records;
    std::string key = "key";
    std::string val = "val";

    GIVEN("An instance of Cache class")
    {
        using namespace libkeen::internal;
        
        Cache cache;

        REQUIRE(cache.connected());
        
        cache.clear();
        REQUIRE(cache.count() == 0);

        THEN("A record is pushed")
        {
            cache.push(key, val);
            REQUIRE(cache.count() == 1);
            REQUIRE(cache.exists(key, val));
        }

        THEN("A record is popped")
        {
            cache.push(key, val);
            cache.pop(records, 10);

            REQUIRE(records.size() == 1);
            REQUIRE(records.back().first == key);
            REQUIRE(records.back().second == val);
            REQUIRE(cache.exists(key, val));
        }

        THEN("A record is removed")
        {
            cache.push(key, val);
            REQUIRE(cache.exists(key, val));

            cache.remove(key, val);
            REQUIRE(cache.count() == 0);
            REQUIRE(!cache.exists(key, val));
        }

        THEN("50 records are pushed single threaded")
        {
            REQUIRE(cache.count() == 0);

            int count = 50;
            for (int index = 0; index < count; ++index)
                cache.push(key + std::to_string(index), val + std::to_string(index));

            REQUIRE(cache.count() == count);

            cache.clear();
            REQUIRE(cache.count() == 0);
        }

        THEN("50 records are pushed multi threaded")
        {
            REQUIRE(cache.count() == 0);

            asio::io_service service;

            int count = 50;
            for (int index = 0; index < count; ++index)
                service.post([=, &cache] { cache.push(key + std::to_string(index), val + std::to_string(index)); });

            std::vector<std::thread> pool;
            for (unsigned t = 0; t < std::thread::hardware_concurrency(); ++t)
                pool.push_back(std::thread([&service] { service.run(); }));

            service.run();

            for (auto& worker : pool)
                if (worker.joinable())
                    worker.join();

            REQUIRE(cache.count() == count);
        }
    }
}
