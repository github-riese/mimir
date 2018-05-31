#ifndef SAMPLER_H
#define SAMPLER_H

#include <map>
#include <vector>

#include "../models/InterpolationStrategy.h"
#include "../models/Sample.h"
#include "../models/SampleStore.h"
#include "../models/ValueType.h"
#include "../models/ValueIndex.h"

namespace mimir {
namespace services {

class Sampler
{
public:
    Sampler();
    explicit Sampler(mimir::models::ValueIndex nameIndex, mimir::models::InterpolationStrategy interpolationStrategy = mimir::models::InterpolationStrategy::CantInterpolate);
    inline mimir::models::ValueIndex nameIndex() const { return _nameIndex; }
    void addSample(mimir::models::Sample);
    unsigned long total() const;
    unsigned long countInValue(mimir::models::ValueIndex value) const;
    unsigned long countInClass(mimir::models::ValueIndex classifier) const;
    unsigned long count(mimir::models::ValueIndex classifier, mimir::models::ValueIndex value) const;
    std::vector<mimir::models::ValueIndex> allClasses() const;
    mimir::models::InterpolationStrategy interpolationStrategy() const;

private:
    mimir::models::ValueIndex _nameIndex;
    mimir::models::InterpolationStrategy _interpolationStrategy;
    mimir::models::SampleStore _samples;
};

} // namespace services
} // namespace mimir

#endif // SAMPLER_H
