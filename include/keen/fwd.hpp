#pragma once

#include <memory>

namespace libkeen
{

class Cache;
class Client;
class Config;

namespace internal
{

class Core;
class Curl;
class Logger;

}

using CacheRef  = std::shared_ptr< Cache >;
using CoreRef   = std::shared_ptr< internal::Core >;
using CurlRef   = std::shared_ptr< internal::Curl >;
using LoggerRef = std::shared_ptr< internal::Logger >;

}
