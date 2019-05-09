#include "Logger.h"
#include "ConsoleLogSink.h"

namespace logger {

Logger::Logger() :
    _sink(new ConsoleLogSink)
{
}

Logger::~Logger() noexcept
{
    delete _sink;
}

void Logger::operator <<(const LogMessage &message) noexcept
{
    writeMessage(_defaultLoglevel, message);
}

void Logger::writeMessage(LogLevel level, const LogMessage &message) noexcept
{
    if (level > _level || _sink == nullptr) {
        return;
    }
    _sink->writeMessage(level, message);
}

} // namespace logger
