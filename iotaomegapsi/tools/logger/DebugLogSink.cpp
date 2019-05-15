#include "DebugLogSink.h"
#include <chrono>
#include <string>
#include <iomanip>

#include "SinkManager.h"

namespace iotaomegapsi {
namespace tools {
namespace logger {

REGISTER_SINK(debug, DebugLogSink);

bool DebugLogSink::writeMessage(const LogMessage &m) noexcept
{
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    _messages << std::put_time(std::localtime(&time), "[%Y-%m-%d %H:%I:%S] ");
    _messages << m.message() << std::endl;
    return true;
}

std::vector<std::string> DebugLogSink::messages() noexcept
{
    std::vector<std::string> lines;
    if (_messages.eof())
        return lines;
    _messages.seekg(0);
    for (std::string line; std::getline(_messages, line);) {
        lines.push_back(line);
    }
    return lines;
}

} // namespace logger
} // namespace tools
} // namespace iotaomegapsi
