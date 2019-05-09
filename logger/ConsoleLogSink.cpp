#include "ConsoleLogSink.h"

#include <iostream>

namespace logger {

ConsoleLogSink::ConsoleLogSink()
{

}

bool ConsoleLogSink::writeMessage(LogLevel level, const LogMessage &message) noexcept
{
    std::ostream &stream = level < LogLevel::Information ? std::cerr : std::cout;
    stream << message;
    stream << std::endl;
    return true;
}

} // namespace logger
