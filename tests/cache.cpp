#include "common.hpp"
#include "keen.hpp"

#include <vector>
#include <string>

TEST_CASE("Cache Check", "[cache]")
{
    using namespace libkeen;

    Cache cache_ref;
    
    REQUIRE(cache_ref.connected());

    cache_ref.clear();

    std::string event_name = "name1";
    std::string event_data = "data1";
    
    std::string event_name_2 = "name2";
    std::string event_data_2 = "data2";

    cache_ref.push(event_name, event_data);
    REQUIRE(cache_ref.exists(event_name, event_data));

    std::vector<std::pair<std::string, std::string>> holder;
    cache_ref.pop(holder, 1);

    REQUIRE(holder.size() == 1);
    REQUIRE(holder.back().first == event_name);
    REQUIRE(holder.back().second == event_data);

    cache_ref.pop(holder, 10);
    REQUIRE(holder.size() == 1);
    REQUIRE(holder.back().first == event_name);
    REQUIRE(holder.back().second == event_data);

    cache_ref.push(event_name_2, event_data_2);
    REQUIRE(cache_ref.exists(event_name_2, event_data_2));

    cache_ref.pop(holder, 10);
    REQUIRE(holder.size() == 2);

    REQUIRE(holder.front().first == event_name);
    REQUIRE(holder.front().second == event_data);
    REQUIRE(holder.back().first == event_name_2);
    REQUIRE(holder.back().second == event_data_2);

    cache_ref.remove(event_name, event_data);
    REQUIRE(!cache_ref.exists(event_name, event_data));
}
