#ifndef LOGSINK_H
#define LOGSINK_H

#include <memory>
#include <string>

#include <iotaomegapsi/iotaomegapsi_global.h>

#include "LogMessage.h"
#include "LogLevel.h"

namespace iotaomegapsi {
namespace tools {
namespace logger {

class TOOLS_EXPORT AbstractLogSink
{
public:
    virtual ~AbstractLogSink() noexcept = default;
    virtual bool writeMessage(LogMessage const &) noexcept = 0;
};

typedef std::shared_ptr<AbstractLogSink> SharedLogSink_t;

} // namespace logger
} // namespace tools
} // namespace iotaomegapsi

#endif // LOGSINK_H
