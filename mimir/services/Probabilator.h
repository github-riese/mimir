#ifndef PROBABILATOR_H
#define PROBABILATOR_H

#include <string>
#include <vector>

#include "../models/Probability.h"
#include "../models/ValueIndex.h"
#include "Sampler.h"

namespace mimir {
namespace services {

class Probabilator
{
public:
    Probabilator(const std::string&);

    void addSampler(const mimir::services::Sampler &);

    mimir::models::Probability evaluate(mimir::models::ValueIndex classification, mimir::models::ValueIndex value) const;
private:
    mimir::models::Probability calculate(const mimir::services::Sampler &, mimir::models::ValueIndex, mimir::models::ValueIndex) const;
private:
    std::string _name;
    std::vector<mimir::services::Sampler> _samplers;
};

} // namespace services
} // namespace mimir

#endif // PROBABILATOR_H
