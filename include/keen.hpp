#pragma once

#include <sstream>
#include <vector>
#include <vector>
#include <memory>
#include <string>
#include <atomic>
#include <thread>
#include <mutex>

struct sqlite3;

namespace libkeen
{

class Client;

namespace internal
{

class Core;
class Curl;
class Cache;
class Logger;

}

using CoreRef   = std::shared_ptr< internal::Core >;
using CurlRef   = std::shared_ptr< internal::Curl >;
using CacheRef  = std::shared_ptr< internal::Cache >;
using LoggerRef = std::shared_ptr< internal::Logger >;

}
