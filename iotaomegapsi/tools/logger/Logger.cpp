#include <memory>
#include <cstdarg>

#include "Logger.h"
#include "SinkManager.h"

namespace iotaomegapsi {
namespace tools {
namespace logger {

static class defaultLoggerFactory {
    std::shared_ptr<Logger> _sharedLogger;
public:
    Logger &fetch()
    {
        if (!_sharedLogger)
            _sharedLogger.reset(new Logger);
        return *_sharedLogger;
    }
    void set(Logger &logger)
    {
        _sharedLogger.reset(&logger);
    }
} defaultLoggerFactory;

Logger &Logger::logger()
{
    return  defaultLoggerFactory.fetch();
}

void Logger::setDefaultLogger(Logger &logger)
{
    defaultLoggerFactory.set(logger);
}

Logger::Logger(LogLevel level) :
    _level(level),
    _sink(SinkManager::sinkManager().defaultSink())
{
}

Logger::Logger(SharedLogSink_t sink, LogLevel level) :
    _level(level),
    _sink(sink)
{
}

Logger::~Logger() noexcept
{
}

void Logger::operator <<(const LogMessage &message) noexcept
{
    writeMessage(message);
}

void Logger::writeMessage(const LogMessage &message) noexcept
{
    LogLevel ll = message.level();
    if (ll == LogLevel::Default) ll = _defaultLoglevel;
    if (ll > _level || _sink == nullptr)
        return;

    LogMessage m(ll, nullptr);
    m.add(message.message());
    _sink->writeMessage(m);
}

void Logger::writeFormattedText(LogLevel level, const std::string format, ...) noexcept
{
    va_list varArgs;
    va_start(varArgs, format);
    writeMessageFwd(level, format, varArgs);
    va_end(varArgs);
}

LogMessage Logger::error() noexcept
{
    return LogMessage(LogLevel::Error, this);
}

LogMessage Logger::warn() noexcept
{
    return LogMessage(LogLevel::Warning, this);
}

LogMessage Logger::info() noexcept
{
    return LogMessage(LogLevel::Information, this);
}

LogMessage Logger::verbose() noexcept
{
    return LogMessage(LogLevel::Verbose, this);
}

LogMessage Logger::debug() noexcept
{
    return LogMessage(LogLevel::Debug, this);
}

void Logger::error(const std::string format, ...) noexcept
{
    va_list args;
    va_start(args, format);
    writeMessageFwd(LogLevel::Error, format, args);
    va_end(args);
}

void Logger::warn(const std::string format, ...) noexcept
{
    va_list args;
    va_start(args, format);
    writeMessageFwd(LogLevel::Warning, format, args);
    va_end(args);
}

void Logger::info(const std::string format, ...) noexcept
{
    va_list args;
    va_start(args, format);
    writeMessageFwd(LogLevel::Information, format, args);
    va_end(args);
}

void Logger::verbose(const std::string format, ...) noexcept
{
    va_list args;
    va_start(args, format);
    writeMessageFwd(LogLevel::Verbose, format, args);
    va_end(args);
}

void Logger::debug(const std::string format, ...) noexcept
{
    va_list args;
    va_start(args, format);
    writeMessageFwd(LogLevel::Debug, format, args);
    va_end(args);
}

void Logger::writeMessageFwd(LogLevel level, const std::string &format, va_list args) noexcept
{
    LogLevel l = level == LogLevel::Default ? _defaultLoglevel : level;
    if (l > _level) {
        return;
    }
    const char *cFormat = format.c_str();
    va_list copiedArgs;
    va_copy(copiedArgs, args);
    int totalLength = vsnprintf(nullptr, 0u, cFormat, copiedArgs);
    va_end(copiedArgs);
    if (totalLength <= 0)
        return;
    std::vector<char> buffer(static_cast<size_t>(totalLength + 1));
    std::vsnprintf(buffer.data(), buffer.size(), cFormat, args);
    LogMessage m(level, nullptr);
    m << std::string(buffer.begin(), buffer.end());
    _sink->writeMessage(m);
}

} // namespace logger
} // namespace tools
} // namespace iotaomegapsi
