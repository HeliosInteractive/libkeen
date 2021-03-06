#pragma once

#include "keen.hpp"

#ifndef LIBKEEN_LOG_TO_CONSOLE
#   define LIBKEEN_LOG_TO_CONSOLE 1
#endif

#ifndef LIBKEEN_LOG_TO_LOGFILE
#   define LIBKEEN_LOG_TO_LOGFILE 1
#endif

namespace libkeen {
namespace internal {

/*!
 * @class Logger
 * @brief Very simple thread-safe logger.
 * @note  Appends logs to both a file and cout/cerr
 * @note  Users must not use this class directly. Users
 *        must use either one of these macros:
 *        LOG_DEBUG, LOG_ERROR, LOG_WARN, or LOG_INFO
 * @note  input to LOG_XX is a C++ style stream:
 *        LOG_WARN("sample " << variable << " stream");
 */
class Logger
{
public:
    ~Logger();
    Logger              (const std::string& type);
    void                log(const std::string& message);
    void                enableLogToFile(bool on = true);
    void                enableLogToConsole(bool on = true);
    static void         pull(std::vector<LoggerRef>& container);

private:
    std::mutex          mMutex;
    const std::string   mType;
    bool                mLogToConsole;
    bool                mLogToFile;
};

namespace loggers
{

std::shared_ptr<Logger> debug();
std::shared_ptr<Logger> error();
std::shared_ptr<Logger> warn();
std::shared_ptr<Logger> info();

#define LOG_DEBUG(stream)   { std::stringstream ss; ss << stream << " [" << __FILE__ << "@ln" << __LINE__ << "]"; libkeen::internal::loggers::debug()->log(ss.str()); }
#define LOG_ERROR(stream)   { std::stringstream ss; ss << stream << " [" << __FILE__ << "@ln" << __LINE__ << "]"; libkeen::internal::loggers::error()->log(ss.str()); }
#define LOG_WARN(stream)    { std::stringstream ss; ss << stream << " [" << __FILE__ << "@ln" << __LINE__ << "]"; libkeen::internal::loggers::warn()->log(ss.str()); }
#define LOG_INFO(stream)    { std::stringstream ss; ss << stream << " [" << __FILE__ << "@ln" << __LINE__ << "]"; libkeen::internal::loggers::info()->log(ss.str()); }

}

}}
