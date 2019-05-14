#ifndef LOGGER_H
#define LOGGER_H

#include <memory>

#include <iotaomegapsi/iotaomegapsi_global.h>

#include "AbstractLogsink.h"
#include "LogMessage.h"
#include "LogLevel.h"

namespace iotaomegapsi {
namespace tools {
namespace logger {

class TOOLS_EXPORT Logger
{
public:
    static Logger &logger();
    static void setDefaultLogger(Logger &);
    Logger(LogLevel level = LogLevel::Information);
    Logger(SharedLogSink_t sink, LogLevel level = LogLevel::Information);
    ~Logger() noexcept;

    inline LogLevel logLevel() const noexcept { return _level; }
    inline void setLoglevel(LogLevel level) noexcept { _level = level; }

    template <typename T>
    LogMessage operator <<(T const &v) noexcept
    {
        LogMessage m(_level, this);
        m << v;
        return m;
    }

    void operator <<(LogMessage const &message) noexcept;
    void writeMessage(LogMessage const &message) noexcept;
    void writeFormattedText(LogLevel level, const std::string format, ...) noexcept;

    void error(const std::string format, ...) noexcept;
    void warn(std::string const format, ...) noexcept;
    void info(std::string const format, ...) noexcept;
    void verbose(std::string const format, ...) noexcept;
    void debug(std::string const format, ...) noexcept;
private:
    void writeMessageFwd(LogLevel level, const std::string &format, va_list) noexcept;
private:
    LogLevel _level = LogLevel::Debug;
    LogLevel _defaultLoglevel = LogLevel::Information;
    SharedLogSink_t _sink;
};

} // namespace logger
} // namespace tools
} // namespace iotaomegapsi

#endif // LOGGER_H
