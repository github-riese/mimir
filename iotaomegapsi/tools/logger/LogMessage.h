#ifndef LOGMESSAGE_H
#define LOGMESSAGE_H

#include <string>
#include <sstream>

#include <iotaomegapsi/iotaomegapsi_global.h>
#include <iotaomegapsi/tools/logger/LogLevel.h>

namespace iotaomegapsi {
namespace tools {
namespace logger {

class Logger;
class TOOLS_EXPORT LogMessage {
public:

    LogMessage() = default;
    LogMessage(LogLevel level, Logger* logger);
    LogMessage(LogMessage const &previous);
    LogMessage & operator=(LogMessage const &);
    LogMessage(LogMessage &&) = default;
    LogMessage & operator =(LogMessage &&) = default;

    ~LogMessage() noexcept;

    inline LogLevel level() const noexcept {
        return _level;
    }

    inline void setLevel(LogLevel level) noexcept {
        _level = level;
    }

    inline std::string message() const {
        return _message.str();
    }

    template<typename T>
    void add(T const &v) noexcept
    {
        *this << v;
    }

    template<typename T>
    LogMessage &operator <<(T const &v) noexcept
    {
        _message << v;
        return *this;
    }

    inline void reset() noexcept
    {
        _message.str(std::string());
    }
private:
    LogLevel _level = LogLevel::Default;
    std::stringstream _message;
    Logger *_logger = nullptr;
};

} // namespace logger
} // namespace tools
} // namespace iotaomegapsi

inline std::ostream &operator<<(std::ostream &stream, iotaomegapsi::tools::logger::LogMessage const &m) noexcept
{
    stream << m.message();
    return stream;
}

#endif // LOGMESSAGE_H
