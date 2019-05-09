#include "LogMessage.h"
#include "Logger.h"

namespace logger {

LogMessage::LogMessage(Logger *logger) :
    _logger(logger)
{

}

LogMessage::~LogMessage() noexcept
{
    if (_logger != nullptr) {
        _logger->writeMessage(LogLevel::Default, *this);
    }
}


}
