#include "LogMessage.h"
#include "Logger.h"

namespace iotaomegapsi {
namespace tools {
namespace logger {

LogMessage::LogMessage(LogLevel level, Logger *logger) :
    _level(level),
    _logger(logger)
{
}

LogMessage::LogMessage(const LogMessage &previous) :
    _level(previous._level),
    _logger(previous._logger)
{
    _message.str(previous.message());
}

LogMessage &LogMessage::operator=(const LogMessage &rhs)
{
    _level = rhs._level;
    _message.str(rhs.message());
    _logger = rhs._logger;
    return *this;
}

LogMessage::~LogMessage() noexcept
{
    if (_logger != nullptr) {
        _logger->writeMessage(*this);
    }
}


}
} // namespace tools
} // namespace iotaomegapsi
