#include "Sample.h"

namespace mimir {
namespace models {

Sample::Sample(ValueIndex classifier, ValueIndex valueIndex, unsigned long count) :
    _classifier(classifier),
    _valueIndex(valueIndex),
    _count(count)
{
}

ValueIndex Sample::classifier() const
{
    return _classifier;
}

ValueIndex Sample::valueIndex() const
{
    return _valueIndex;
}

unsigned long Sample::count() const
{
    return _count;
}

} // namespace models
} // namespace mimir
