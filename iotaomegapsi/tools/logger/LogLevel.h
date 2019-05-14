#ifndef LOGLEVEL_H
#define LOGLEVEL_H

namespace iotaomegapsi {
namespace tools {
namespace logger {
enum class LogLevel : int
{
    Default = -1,
    Quiet = 0,
    Error,
    Warning,
    Information,
    Verbose,
    Debug
};

}
} // namespace tools
} // namespace iotaomegapsi

#endif // LOGLEVEL_H
