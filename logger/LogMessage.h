#ifndef LOGMESSAGE_H
#define LOGMESSAGE_H

#include <string>
#include <sstream>

#include "LogLevel.h"

namespace logger {

class Logger;
class LogMessage {
public:

    LogMessage(Logger* logger);

    LogMessage()
    {}

    ~LogMessage() noexcept;

    LogMessage(LogMessage const &previous)
    {
        _message.str(previous.message());
    }

    inline std::string message() const {
        return _message.str();
    }

    template<typename T>
    void add(T const &v) noexcept
    {
        _message << v;
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
    std::stringstream _message;
    Logger *_logger = nullptr;
};

} // namespace logger

inline std::ostream &operator<<(std::ostream &stream, logger::LogMessage const &m) noexcept
{
    stream << m.message();
    return stream;
}

#endif // LOGMESSAGE_H
