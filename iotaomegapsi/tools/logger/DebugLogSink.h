#ifndef DEBUGLOGSINK_H
#define DEBUGLOGSINK_H

#include <sstream>
#include <vector>

#include <iotaomegapsi/iotaomegapsi_global.h>
#include "AbstractLogsink.h"
#include "LogMessage.h"

namespace iotaomegapsi {
namespace tools {
namespace logger {

class TOOLS_EXPORT DebugLogSink : public AbstractLogSink
{
public:
    DebugLogSink() = default;
    DebugLogSink(DebugLogSink const &) = delete;
    DebugLogSink(DebugLogSink &&) = default;
    DebugLogSink &operator =(DebugLogSink const &) = delete;
    DebugLogSink &operator =(DebugLogSink &&) = default;
    ~DebugLogSink() override = default;
    bool writeMessage(const LogMessage &) noexcept override;
    std::vector<std::string> messages() noexcept;
private:
    std::stringstream _messages;
};

} // namespace logger
} // namespace tools
} // namespace iotaomegapsi

#endif // DEBUGLOGSINK_H
