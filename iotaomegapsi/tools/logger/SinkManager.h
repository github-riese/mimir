#ifndef SINKMANAGER_H
#define SINKMANAGER_H

#include <string>
#include <vector>

#include <iotaomegapsi/iotaomegapsi_global.h>

#include "AbstractLogsink.h"

namespace iotaomegapsi {
namespace tools {
namespace logger {

class TOOLS_EXPORT SinkManager
{
    struct SinkDesc {
        std::string name;
        bool isDefault;
        SharedLogSink_t sink;
    };
public:
    static SinkManager &sinkManager();
    SinkManager();
    SharedLogSink_t defaultSink() const;
    SharedLogSink_t namedSink(std::string const &) const;
    void registerSink(std::string const &name, SharedLogSink_t sink);
    bool makeDefault(std::string const &name);
private:
    std::vector<SinkDesc> _knownSinks;
};
} // namespace logger
} // namespace tools
} // namespace iotaomegapsi

template<typename sink>
class sinkregisterer
{
public:
    sinkregisterer(std::string const &name)
    {
        iotaomegapsi::tools::logger::SinkManager::sinkManager().registerSink(name, std::make_shared<sink>());
    }
};
#define REGISTER_SINK(friendlyName, SinkType) static sinkregisterer<SinkType> _sinkTypeRegisterer_ ## SinkType(#friendlyName);

#endif // SINKMANAGER_H
