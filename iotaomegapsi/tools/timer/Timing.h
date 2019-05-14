#ifndef TIMING_H
#define TIMING_H

#include <chrono>
#include <iostream>
#include <string>

#include <iotaomegapsi/tools/logger/Logger.h>

namespace iotaomegapsi {
namespace tools {
namespace timer {

template<class Duration>
inline constexpr const char* unit();

template<>inline constexpr const char* unit<std::chrono::nanoseconds>() { return "ns"; }
template<>inline constexpr const char* unit<std::chrono::microseconds>() { return "us"; }
template<>inline constexpr const char* unit<std::chrono::milliseconds>() { return "ms"; }
template<>inline constexpr const char* unit<std::chrono::seconds>() { return "s"; }

class Timing
{
public:
    inline void restart() {
        _started = std::chrono::steady_clock::now();
    }

    template<typename Duration = std::chrono::microseconds>
    Duration sinceStart() const
    {
        return std::chrono::duration_cast<Duration>(
                    std::chrono::steady_clock::now() - _started
                );
    }

private:
    std::chrono::steady_clock::time_point _started = std::chrono::steady_clock::now();
};

template<typename Duration = std::chrono::milliseconds>
class VerboseTiming : public Timing
{
public:
    inline VerboseTiming(const std::string &name, std::ostream &stream = std::cerr) :
        Timing(),
        _name(name),
        _stream(stream) {}
    inline void report() const {
        auto duration = sinceStart<Duration>();
        _stream << "[Timer] \"" << _name << "\": " << duration.count() << unit<Duration>() << " elapsed." << std::endl;
    }
    inline ~VerboseTiming() {
        report();
    }
private:
    std::string _name;
    std::ostream &_stream;
};

template<typename Duration = std::chrono::milliseconds>
class LoggingTiming : public Timing
{
public:
    inline LoggingTiming(const std::string &name, logger::Logger &logger = logger::Logger::logger()) :
        Timing(),
        _name(name),
        _logger(logger) {}
    inline void report() const {
        auto duration = sinceStart<Duration>();
        _logger << "[Timer] \"" << _name << "\": " << duration.count() << unit<Duration>() << " elapsed.";
    }
    inline ~LoggingTiming() {
        report();
    }
private:
    std::string _name;
    logger::Logger &_logger;
};



} // namespace timer
} // namespace tools
} // namespace iotaomegapsi

#endif // TIMING_H
