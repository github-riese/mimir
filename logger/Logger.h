#ifndef LOGGER_H
#define LOGGER_H

#include "AbstractLogsink.h"
#include "LogMessage.h"
#include "LogLevel.h"

namespace logger {

class Logger
{
public:
    Logger();
    ~Logger() noexcept;

    inline LogLevel logLevel() const noexcept { return _level; }
    inline void setLoglevel(LogLevel level) noexcept { _level = level; }

    template <typename T>
    LogMessage operator <<(T const &v) noexcept
    {
        LogMessage m(this);
        m << v;
        return m;
    }
    void operator <<(LogMessage const &message) noexcept;
    void writeMessage(LogLevel level, LogMessage const &message) noexcept;
    inline void error(LogMessage const &message) noexcept { writeMessage(LogLevel::Error, message); }
    inline void warn(LogMessage const &message) noexcept { writeMessage(LogLevel::Warning, message); }
    inline void info(LogMessage const &message) noexcept { writeMessage(LogLevel::Information, message); }
    inline void verbose(LogMessage const &message) noexcept { writeMessage(LogLevel::Verbose, message); }
    inline void debug(LogMessage const &message) noexcept { writeMessage(LogLevel::Debug, message); }
private:
    LogLevel _level = LogLevel::Debug;
    LogLevel _defaultLoglevel = LogLevel::Information;
    AbstractLogsink *_sink = nullptr;
};

} // namespace logger

#endif // LOGGER_H
