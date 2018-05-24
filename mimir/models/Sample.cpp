#include "Sample.h"

namespace mimir {
namespace models {

Sample::Sample(size_t classifier, size_t value, unsigned count) :
    _classifier(classifier),
    _value(value),
    _count(count)
{
}

size_t Sample::classifier() const
{
    return _classifier;
}

size_t Sample::value() const
{
    return _value;
}

unsigned Sample::count() const
{
    return _count;
}

} // namespace models
} // namespace mimir
