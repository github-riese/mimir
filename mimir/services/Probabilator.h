#ifndef PROBABILATOR_H
#define PROBABILATOR_H

#include <string>
#include <vector>

#include "../models/Evaluation.h"
#include "../models/ValueIndex.h"
#include "Sampler.h"

namespace mimir {
namespace services {

class Probabilator
{
public:
    Probabilator(const std::string&);

    void setSampler(const mimir::services::Sampler &);

    mimir::models::Evaluation evaluateSampler(mimir::models::ValueIndex value) const;

    mimir::models::Probability combineProbabilities(const std::vector<mimir::models::Probability>&) const;
private:
    mimir::models::Probability calculate(const mimir::services::Sampler &, mimir::models::ValueIndex, mimir::models::ValueIndex) const;

    long double bayes(unsigned long countInClassAndValue, unsigned long totalInClass, unsigned long totalInValue, unsigned long totalSamples) const;
    long double bayes(long double pBonConditionA, long double pA, long double pB) const;
private:
    std::string _name;
    mimir::services::Sampler _sampler;
};

} // namespace services
} // namespace mimir

#endif // PROBABILATOR_H
