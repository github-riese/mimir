#include "SinkManager.h"
#include "ConsoleLogSink.h"
#include <algorithm>

using std::distance;
using std::find_if;
using std::iter_swap;

namespace iotaomegapsi {
namespace tools {
namespace logger {

static SinkManager _s_manager;

SinkManager &SinkManager::sinkManager()
{
    return _s_manager;
}


class NullSink : public AbstractLogSink
{
public:
    bool writeMessage(const LogMessage &) noexcept override
    {
        return true;
    }
};
REGISTER_SINK(null, NullSink);

SharedLogSink_t SinkManager::defaultSink() const
{
    if (_knownSinks.front().isDefault)
        return _knownSinks.front().sink;
    return namedSink("null");
}

SharedLogSink_t SinkManager::namedSink(const std::string &name) const
{
    SharedLogSink_t *nullSink = nullptr;
    for (auto s : _knownSinks) {
        if (s.name == name)
            return s.sink;
         if (s.name == "null")
            nullSink = &s.sink;
    }
    return *nullSink;
}

void SinkManager::registerSink(const std::string &name, SharedLogSink_t const &sink)
{
    auto current = find_if(_knownSinks.begin(), _knownSinks.end(), [name] (auto current) -> bool {
        return current.name == name;
    });
    if (current == _knownSinks.end()) {
        _knownSinks.push_back({name, false, sink});
    } else {
        current->sink = sink;
    }
}

bool SinkManager::makeDefault(const std::string &name)
{
    auto s = _knownSinks.begin();
    auto found = _knownSinks.end();
    for (; s != _knownSinks.end(); ++s) {
        if (s->name == name)
            found = s;
    }
    if (found == _knownSinks.end()) {
        return false;
    }
    if (found != _knownSinks.begin()) {
        _knownSinks.front().isDefault = false;
        auto offset = distance(_knownSinks.begin(), found);
        iter_swap(_knownSinks.begin(), _knownSinks.begin() + offset);
    }
    _knownSinks.front().isDefault = true;
    return true;
}

} // namespace logger
} // namespace tools
} // namespace iotaomegapsi
