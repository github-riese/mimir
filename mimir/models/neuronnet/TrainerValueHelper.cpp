#include "TrainerValueHelper.h"

namespace mimir {
namespace models {


TrainerValueHelper::TrainerValueHelper(const std::vector<float> &hypothesis, const std::vector<float> &expectation) :
    _hypothesis(hypothesis),
    _expectation(expectation)
{
}

const std::vector<float> &TrainerValueHelper::hypothesis() const
{
    return _hypothesis;
}

void TrainerValueHelper::setHypothesis(const std::vector<float> &hypothesis)
{
    _hypothesis = hypothesis;
}

const std::vector<float> &TrainerValueHelper::expectation() const
{
    return _expectation;
}

void TrainerValueHelper::setExpectation(const std::vector<float> &expectation)
{
    _expectation = expectation;
}

} // namespace models
} // namespace mimir
