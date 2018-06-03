#include "Sampler.h"
#include <algorithm>
#include <numeric>

using std::find_if;
using std::accumulate;

using std::vector;

using mimir::models::InterpolationStrategy;
using mimir::models::Sample;
using mimir::models::SampleStore;
using mimir::models::ValueCounter;
using mimir::models::ValueIndex;

namespace mimir {
namespace services {

Sampler::Sampler() :
    _nameIndex(),
    _interpolationStrategy(),
    _samples()
{
}

Sampler::Sampler(ValueIndex nameIndex, InterpolationStrategy interpolationStrategy) :
    _nameIndex(nameIndex),
    _interpolationStrategy(interpolationStrategy),
    _samples()
{
}

void Sampler::addSample(Sample sample)
{
    _samples.add(sample);
}

unsigned long Sampler::total() const
{
    unsigned long total(0);
    for (auto classes : _samples) {
        total += accumulate(classes.second.cbegin(), classes.second.cend(), 0UL);
    }
    return total;
}

unsigned long Sampler::countInValue(ValueIndex value) const
{
    unsigned total = 0;
    for (auto classes : _samples) {
        for (auto values : classes.second) {
            if (values.valueIndex() == value) {
                total += values.count();
            }
        }
    }
    return total;
}

unsigned long Sampler::countInClass(ValueIndex classifier) const
{
    auto classPtr = _samples.find(classifier);
    if (classPtr == _samples.end()) {
        return 0;
    }
    return accumulate((*classPtr).second.begin(), (*classPtr).second.end(), 0UL);
}

unsigned long Sampler::count(ValueIndex classifier, ValueIndex value) const
{
    unsigned total = 0;
    auto classPtr = _samples.find(classifier);
    if (classPtr == _samples.end()) {
        return 0;
    }
    for (auto values : (*classPtr).second) {
        if (values.valueIndex() == value) {
            total += values.count();
        }
    }
    return total;
}

std::vector<models::ValueIndex> Sampler::allClasses() const
{
    vector<ValueIndex> result;
    for (auto v : _samples) {
        result.push_back(v.first);
    }
    return result;
}

mimir::models::InterpolationStrategy Sampler::interpolationStrategy() const
{
    return _interpolationStrategy;
}

} // namespace services
} // namespace mimir
