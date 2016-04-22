#include "internal/logger.hpp"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <ctime>

// by default turn on both file and console
// logging behaviors.

#ifndef LIBKEEN_LOG_TO_CONSOLE
#   define LIBKEEN_LOG_TO_CONSOLE 1
#endif

#ifndef LIBKEEN_LOG_TO_LOGFILE
#   define LIBKEEN_LOG_TO_LOGFILE 1
#endif

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

Logger::~Logger()
{
    log("Logger " + mType + " is shutdown.");
}

Logger::Logger(const std::string& type)
    : mType(type)
    , mLogToFile(true)
    , mLogToConsole(true)
{
    log("Logger " + mType + " is started.");
}

void Logger::log(const std::string& message)
{
#if LIBKEEN_LOG_TO_CONSOLE || LIBKEEN_LOG_TO_LOGFILE

    try {
        std::lock_guard<decltype(mMutex)> lock(mMutex);
        std::stringstream msg;
        msg << "[" << mType << "]["
            << std::this_thread::get_id() << "]["
            << now() << "]: "
            << message << std::endl;

#if LIBKEEN_LOG_TO_CONSOLE
        if (mLogToConsole)
        {
            std::cout << msg.str();
        }
#endif // LIBKEEN_LOG_TO_CONSOLE

#if LIBKEEN_LOG_TO_LOGFILE
        if (mLogToFile)
        {
            std::ofstream("libkeen.log", std::ios_base::app | std::ios_base::out) << msg.str();
        }
#endif // LIBKEEN_LOG_TO_LOGFILE

    }
    catch (const std::exception& ex) {
        std::cerr << "Logger failed: " << ex.what() << std::endl;
    }
    catch (...) {
        std::cerr << "Logger failed." << std::endl;
    }

#endif
}

void Logger::enableLogToFile(bool on /*= true*/)
{
    std::lock_guard<decltype(mMutex)> lock(mMutex);
    mLogToFile = on;
}

void Logger::enableLogToConsole(bool on /*= true*/)
{
    std::lock_guard<decltype(mMutex)> lock(mMutex);
    mLogToConsole = on;
}

void Logger::pull(std::vector<LoggerRef>& container)
{
    if (!container.empty())
        container.clear();

    container.push_back(loggers::debug());
    container.push_back(loggers::error());
    container.push_back(loggers::warn());
    container.push_back(loggers::info());
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
