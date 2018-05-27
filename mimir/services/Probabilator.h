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

    void addSampler(const mimir::services::Sampler &);

    mimir::models::Evaluation evaluate(mimir::models::ValueIndex value) const;

    mimir::models::Probability metaProbability(const std::vector<mimir::models::Probability>&) const;
private:
    mimir::models::Probability calculate(const mimir::services::Sampler &, mimir::models::ValueIndex, mimir::models::ValueIndex) const;
    inline long double bayes(unsigned long countInClassAndValue, unsigned long totalInClass, unsigned long totalInValue, unsigned long totalSamples) const
    {
        return
                (
                    ((long double)countInClassAndValue / (long double) totalInClass) *
                    ((long double)totalInClass / (long double) totalSamples)
                ) /
                ((long double) totalInValue / (long double) totalSamples);
    }
private:
    std::string _name;
    std::vector<mimir::services::Sampler> _samplers;
};

} // namespace services
} // namespace mimir

#endif // PROBABILATOR_H
