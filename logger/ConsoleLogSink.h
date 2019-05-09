#ifndef CONSOLELOGSINK_H
#define CONSOLELOGSINK_H

#include "AbstractLogsink.h"

namespace logger {

class ConsoleLogSink : public AbstractLogsink
{
public:
    ConsoleLogSink();
    bool writeMessage(LogLevel, const LogMessage &) noexcept override;
};

} // namespace logger

#endif // CONSOLELOGSINK_H
