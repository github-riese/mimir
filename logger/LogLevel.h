#ifndef LOGLEVEL_H
#define LOGLEVEL_H

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
#endif // LOGLEVEL_H
