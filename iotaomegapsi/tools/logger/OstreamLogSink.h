#ifndef OSTREAMLOGSINK_H
#define OSTREAMLOGSINK_H

#include <ostream>

#include <iotaomegapsi/iotaomegapsi_global.h>

#include "AbstractLogsink.h"

namespace iotaomegapsi {
namespace tools {
namespace logger {

class TOOLS_EXPORT OstreamLogSink : public AbstractLogSink
{
public:
    OstreamLogSink(std::ostream &theStream);

    bool writeMessage(const LogMessage &) noexcept override;
private:
    std::ostream &_stream;
};

} // namespace logger
} // namespace tools
} // namespace iotaomegapsi

#endif // OSTREAMLOGSINK_H
