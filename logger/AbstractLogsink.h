#ifndef LOGSINK_H
#define LOGSINK_H

#include "LogMessage.h"
#include "LogLevel.h"

namespace logger {

class AbstractLogsink
{
public:
    virtual ~AbstractLogsink() noexcept = default;
    virtual bool writeMessage(LogLevel, LogMessage const &) noexcept = 0;
};

} // namespace logger

#endif // LOGSINK_H
