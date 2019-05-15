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
    AbstractLogSink() = default;
    AbstractLogSink(AbstractLogSink const &) = delete;
    AbstractLogSink(AbstractLogSink &&) = default;
    AbstractLogSink& operator=(AbstractLogSink &&) = default;
    AbstractLogSink& operator=(AbstractLogSink const &) = delete;
    virtual ~AbstractLogSink() noexcept = default;
    virtual bool writeMessage(LogMessage const &) noexcept = 0;
};

using SharedLogSink_t = std::shared_ptr<AbstractLogSink>;

} // namespace logger
} // namespace tools
} // namespace iotaomegapsi

#endif // LOGSINK_H
