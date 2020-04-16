#include "BatchItem.h"

namespace mimir {
namespace models {

BatchItem::BatchItem(const std::vector<float> &input, const std::vector<float> &expectation) :
    _input(input),
    _expectation(expectation)
{
}

std::vector<float> const &BatchItem::input() const
{
    return _input;
}

void BatchItem::setInput(const std::vector<float> &input)
{
    _input = input;
}

const std::vector<float> &BatchItem::expectation() const
{
    return _expectation;
}

void BatchItem::setExpectation(const std::vector<float> &expectation)
{
    _expectation = expectation;
}

} // namespace models
} // namespace mimir
