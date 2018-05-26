#include "ValueCounter.h"

namespace mimir {
namespace models {

ValueCounter::ValueCounter() :
    _valueIndex(0),
    _count(0)
{
}

ValueCounter::ValueCounter(ValueIndex valueIndex, unsigned long count) :
    _valueIndex(valueIndex),
    _count(count)
{
}

} // namespace models
} // namespace mimir
