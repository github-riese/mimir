#ifndef MIMIR_SERVICES_EVALUATOR_H
#define MIMIR_SERVICES_EVALUATOR_H

#include <string>
#include <vector>

#include "../models/Evaluation.h"
#include "../models/ValueIndex.h"

namespace mimir {
namespace services {

class Evaluator
{
public:

    mimir::models::Evaluation classify(const std::vector<mimir::models::Evaluation> &);

    inline unsigned long long opcount() const { return _opcount; }

private:
    mimir::models::ProbabilityWithPriors combineProbabilities(const std::vector<mimir::models::Probability>&, models::Probability classProbability, models::Probability evidence);

    std::vector<mimir::models::ValueIndex> combineSources(const std::vector<std::vector<mimir::models::ValueIndex>>&);

    std::vector<std::vector<models::ValueIndex> > combineSources(const std::vector<mimir::models::Evaluation> &);


    mimir::models::Probability calculate(unsigned long countInClass, unsigned long countInValue, unsigned long total);
private:
    unsigned long long _opcount = 0;
};

} // namespace services
} // namespace mimir

#endif // MIMIR_SERVICES_EVALUATOR_H
