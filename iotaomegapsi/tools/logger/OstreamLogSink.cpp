#include "OstreamLogSink.h"

namespace iotaomegapsi {
namespace tools {
namespace logger {

OstreamLogSink::OstreamLogSink(std::ostream &theStream) :
    _stream(theStream)
{

}

bool OstreamLogSink::writeMessage(const LogMessage &message) noexcept
{
    _stream << message;
    _stream << std::endl;
    return true;
}

} // namespace logger
} // namespace tools
} // namespace iotaomegapsi
