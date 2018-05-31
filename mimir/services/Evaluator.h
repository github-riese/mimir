#ifndef MIMIR_SERVICES_EVALUATOR_H
#define MIMIR_SERVICES_EVALUATOR_H

#include <string>
#include <vector>

#include "../models/Evaluation.h"
#include "../models/ValueIndex.h"
#include "Sampler.h"

namespace mimir {
namespace services {

class Evaluator
{
public:
    mimir::models::Evaluation evaluate(const mimir::services::Sampler &sampler, mimir::models::ValueIndex value);

    mimir::models::Evaluation evaluate(const std::vector<mimir::models::Evaluation> &);

    std::vector<mimir::models::ValueIndex> combineSamplerIDs(const std::vector<std::vector<mimir::models::ValueIndex>>&);

    inline unsigned long long opcount() const { return _opcount; }
private:
    mimir::models::Probability combineProbabilities(const std::vector<mimir::models::Probability>&);

    mimir::models::Probability calculate(const mimir::services::Sampler &, mimir::models::ValueIndex, mimir::models::ValueIndex);
private:
    unsigned long long _opcount = 0;
};

} // namespace services
} // namespace mimir

#endif // MIMIR_SERVICES_EVALUATOR_H
