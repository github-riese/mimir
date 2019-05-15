#ifndef CONSOLELOGSINK_H
#define CONSOLELOGSINK_H

#include "../iotaomegapsi_global.h"
#include "AbstractLogsink.h"
#include "SinkManager.h"

namespace iotaomegapsi {
namespace tools {
namespace logger {

class TOOLS_EXPORT ConsoleLogSink : public AbstractLogSink
{
public:
    ConsoleLogSink() = default;
    bool writeMessage(const LogMessage &) noexcept override;
};

} // namespace logger
} // namespace tools
} // namespace iotaomegapsi

#endif // CONSOLELOGSINK_H
