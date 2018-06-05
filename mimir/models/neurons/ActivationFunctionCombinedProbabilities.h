#ifndef ACTIVATIONFUNCTIONCOMBINEDPROBABILITIES_H
#define ACTIVATIONFUNCTIONCOMBINEDPROBABILITIES_H

#include "AbstractActivation.h"

#include "../Evaluation.h"
#include "../Probability.h"

namespace mimir {
namespace models {
namespace neurons {

class ActivationFunctionCombinedProbabilities : public AbstractActivation
{
public:
    ActivationFunctionCombinedProbabilities(long double mean, long double stdDeviation, Probability test);
    bool operator()() const override;
private:
    long double _mean;
    long double _stdDeviation;
    Probability _test;
};

} // namespace neurons
} // namespace models
} // namespace mimir

#endif // ACTIVATIONFUNCTIONCOMBINEDPROBABILITIES_H
