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
    static mimir::models::Evaluation evaluate(const mimir::services::Sampler &sampler, mimir::models::ValueIndex value);

    static mimir::models::Evaluation evaluate(const std::vector<mimir::models::Evaluation> &);

    static std::vector<mimir::models::ValueIndex> combineSamplerIDs(const std::vector<std::vector<mimir::models::ValueIndex>>&);
private:
    static mimir::models::Probability combineProbabilities(const std::vector<mimir::models::Probability>&);

    static mimir::models::Probability calculate(const mimir::services::Sampler &, mimir::models::ValueIndex, mimir::models::ValueIndex);

};

} // namespace services
} // namespace mimir

#endif // PROBABILATOR_H
