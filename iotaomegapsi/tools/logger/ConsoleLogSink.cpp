#include "ConsoleLogSink.h"

#include <iostream>

namespace iotaomegapsi {
namespace tools {
namespace logger {

REGISTER_SINK("console", ConsoleLogSink);

bool ConsoleLogSink::writeMessage(const LogMessage &message) noexcept
{
    std::ostream &stream = message.level() < LogLevel::Information ? std::cerr : std::cout;
    stream << message;
    stream << std::endl;
    return true;
}

} // namespace logger
} // namespace tools
} // namespace iotaomegapsi
