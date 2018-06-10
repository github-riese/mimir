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

Sampler &SamplerManager::createSampler(const string &name, ValueType valueType)
{
    KnownSampler &knownSampler = _samplers[name];
    knownSampler.sampler = Sampler(_nameResolver.indexFromName(name));
    knownSampler.valueType = valueType;
    return knownSampler.sampler;
}

bool SamplerManager::isKnownSampler(const std::string &name) const
{
    return _samplers.find(name) != _samplers.end();
}

Sampler &SamplerManager::sampler(const std::string &name)
{
    KnownSampler &known = _samplers[name];
    return known.sampler;
}

const Sampler &SamplerManager::sampler(const std::string &name) const
{
    static Sampler inValidSampler;
    auto knownSamplerPtr = _samplers.find(name);
    if (knownSamplerPtr == _samplers.end()) {
        inValidSampler = Sampler();
        return inValidSampler;
    }
    return knownSamplerPtr->second.sampler;
}

} // namespace services
} // namespace mimir
