#pragma once

#include "keen/fwd.hpp"

#include <sstream>
#include <vector>
#include <string>
#include <mutex>

namespace libkeen {
namespace internal {

class Logger
{
public:
    ~Logger();
    Logger              (const std::string& type);
    void                log(const std::string& message);
    static void         pull(std::vector<LoggerRef>& container);

private:
    std::mutex          mMutex;
    const std::string   mType;
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
