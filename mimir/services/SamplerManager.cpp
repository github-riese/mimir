#include "SamplerManager.h"

using std::map;
using std::string;

using mimir::models::InterpolationStrategy;
using mimir::models::ValueCounter;
using mimir::models::ValueIndex;
using mimir::models::ValueType;

namespace mimir {
namespace services {

SamplerManager::SamplerManager(NameResolver &resolver) :
    _nameResolver(resolver),
    _samplers()
{
}

Sampler &SamplerManager::createSampler(const string &name, ValueType valueType, InterpolationStrategy interpolationStrategy)
{
    KnownSampler &knownSampler = _samplers[name];
    knownSampler.sampler = Sampler(_nameResolver.indexFromName(NameResolver::NameSource::Sampler, name), interpolationStrategy);
    knownSampler.valueType = valueType;
    return knownSampler.sampler;
}

} // namespace services
} // namespace mimir
