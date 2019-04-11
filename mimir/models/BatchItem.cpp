#include "BatchItem.h"

namespace mimir {
namespace models {

BatchItem::BatchItem(const std::vector<double> &input, const std::vector<double> &expectation) :
    _input(input),
    _expectation(expectation)
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

} // namespace models
} // namespace mimir
