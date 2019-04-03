#include "TrainerValueHelper.h"

namespace mimir {
namespace models {


TrainerValueHelper::TrainerValueHelper(const std::vector<double> &hypothesis, const std::vector<double> &expectation) :
    _hypothesis(hypothesis),
    _expectation(expectation)
{
}

const std::vector<double> &TrainerValueHelper::hypothesis() const
{
    return _hypothesis;
}

void TrainerValueHelper::setHypothesis(const std::vector<double> &hypothesis)
{
    _hypothesis = hypothesis;
}

const std::vector<double> &TrainerValueHelper::expectation() const
{
    return _expectation;
}

void TrainerValueHelper::setExpectation(const std::vector<double> &expectation)
{
    _expectation = expectation;
}

} // namespace models
} // namespace mimir
