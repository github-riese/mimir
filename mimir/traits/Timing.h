#ifndef TIMING_H
#define TIMING_H

#include <chrono>
#include <iostream>
#include <string>

namespace mimir {
namespace traits {

class Timing
{
public:
    inline void restart() {
        _started = std::chrono::steady_clock::now();
    }

    std::chrono::milliseconds millisecsSinceStart() const
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::steady_clock::now() - _started
                );
    }

    std::chrono::microseconds microsecsSinceStart() const
    {
        return std::chrono::duration_cast<std::chrono::microseconds>(
                    std::chrono::steady_clock::now() - _started
        );
    }

    std::chrono::nanoseconds nanosecsSinceStart() const
    {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::steady_clock::now() - _started
        );
    }
private:
    std::chrono::steady_clock::time_point _started = std::chrono::steady_clock::now();
};

class VerboseTiming : public Timing
{
public:
    inline VerboseTiming(const std::string &name, std::ostream &stream = std::cerr) :
        Timing(),
        _name(name),
        _stream(stream) {}
    inline void report() const {
        auto millis = microsecsSinceStart();
        _stream << "[Timer] \"" << _name << "\": " << millis.count() << "us elapsed." << std::endl;
    }
    inline ~VerboseTiming() {
        report();
    }
private:
    std::string _name;
    std::ostream &_stream;
};
} // namespace traits
} // namespace mimir

#endif // TIMING_H
