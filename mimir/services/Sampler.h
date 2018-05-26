#ifndef SAMPLER_H
#define SAMPLER_H

#include <map>
#include <vector>
#include <string>

#include "../models/Sample.h"
#include "../models/SampleStore.h"
#include "../models/ValueIndex.h"

namespace mimir {
namespace services {

class Sampler
{
public:
    explicit Sampler(std::string name);
    void addSample(mimir::models::Sample);
    unsigned long total() const;
    unsigned long countInValue(mimir::models::ValueIndex value) const;
    unsigned long countInClass(mimir::models::ValueIndex classifier) const;
    unsigned long count(mimir::models::ValueIndex classifier, mimir::models::ValueIndex value) const;
private:
    std::string _name;
    mimir::models::SampleStore _samples;
};

} // namespace services
} // namespace mimir

#endif // SAMPLER_H
