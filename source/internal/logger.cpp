#include "logger.hpp"

#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>

namespace
{
std::string now()
{
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&now_time_t), "%Y-%m-%d %X");
    return ss.str();
}}

namespace libkeen {
namespace internal {

Logger::Logger(const std::string& type)
    : mType(type)
{ /* no-op */ }

void Logger::log(const std::string& message)
{
    std::lock_guard<decltype(mMutex)> lock(mMutex);
    std::cout
        << "[" << mType << "]["
        << std::this_thread::get_id() << "]["
        << now() << "]: "
        << message << std::endl;
}

std::shared_ptr<Logger> loggers::debug()
{
    static std::shared_ptr<Logger> debug_logger = std::make_shared<Logger>("debug");
    return debug_logger;
}

std::shared_ptr<Logger> loggers::error()
{
    static std::shared_ptr<Logger> error_logger = std::make_shared<Logger>("error");
    return error_logger;
}

std::shared_ptr<Logger> loggers::warn()
{
    static std::shared_ptr<Logger> warn_logger = std::make_shared<Logger>("warn");
    return warn_logger;
}

std::shared_ptr<Logger> loggers::info()
{
    static std::shared_ptr<Logger> info_logger = std::make_shared<Logger>("info");
    return info_logger;
}

}}
