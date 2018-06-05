#ifndef SAMPLERMANAGER_H
#define SAMPLERMANAGER_H

#include <map>
#include <string>

#include "NameResolver.h"
#include "Sampler.h"

#include "../models/ValueType.h"
#include "../models/InterpolationStrategy.h"

namespace mimir {
namespace services {

class SamplerManager
{
    struct KnownSampler
    {
        mimir::models::ValueType valueType;
        mimir::models::InterpolationStrategy interpolationStrategy;
        mimir::services::Sampler sampler;
    };
public:
    SamplerManager(NameResolver &resolver);
    Sampler &createSampler(const std::string& name, mimir::models::ValueType valueType, mimir::models::InterpolationStrategy interpolationStrategy = mimir::models::InterpolationStrategy::CantInterpolate);
    bool isKnownSampler(const std::string &name) const;
    Sampler &sampler(const std::string &name);
    const Sampler &sampler(const std::string &name) const;
private:
    NameResolver &_nameResolver;
    std::map<std::string, KnownSampler> _samplers;
};

} // namespace services
} // namespace mimir

#endif // SAMPLERMANAGER_H
