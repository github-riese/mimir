#include "ActivationFunctionCombinedProbabilities.h"

namespace mimir {
namespace models {
namespace neurons {

ActivationFunctionCombinedProbabilities::ActivationFunctionCombinedProbabilities(long double mean, long double stdDeviation, Probability testValue) :
    _mean(mean),
    _stdDeviation(stdDeviation),
    _test(testValue)
{

}

bool ActivationFunctionCombinedProbabilities::operator()() const
{
    return abs(_test.probability() - _mean) > _stdDeviation;
}

} // namespace neurons
} // namespace models
} // namespace mimir
