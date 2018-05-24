#ifndef SAMPLER_H
#define SAMPLER_H

#include <map>
#include <vector>
#include <string>

#include "models/Sample.h"
#include "models/SampleStore.h"

using std::string;

using mimir::models::SampleStore;

namespace mimir {
namespace services {

class Sampler
{
public:
    explicit Sampler(string name);
    void addSample(models::Sample);
    unsigned total() const;
    unsigned countIn(size_t value) const;
    unsigned count(size_t classifier, size_t value) const;
private:
    string _name;
    SampleStore _samples;
};

} // namespace services
} // namespace mimir

#endif // SAMPLER_H
