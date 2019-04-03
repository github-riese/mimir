#include "BatchItem.h"

namespace mimir {
namespace models {

BatchItem::BatchItem(const std::vector<double> &input, const std::vector<double> &expectation, const std::vector<double> &hypothesis) :
    _input(input),
    _expectation(expectation),
    _hypothesis(hypothesis)
{
}

std::vector<double> const &BatchItem::input() const
{
    return _input;
}

void BatchItem::setInput(const std::vector<double> &input)
{
    _input = input;
}

const std::vector<double> &BatchItem::expectation() const
{
    return _expectation;
}

void BatchItem::setExpectation(const std::vector<double> &expectation)
{
    _expectation = expectation;
}

const std::vector<double> &BatchItem::hypothesis() const
{
    return _hypothesis;
}

void BatchItem::setHypothesis(const std::vector<double> &hypothesis)
{
    _hypothesis = hypothesis;
}

} // namespace models
} // namespace mimir
