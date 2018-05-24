#include "ValueCounter.h"

ValueCounter::ValueCounter() :
    pair(0, 0)
{
}

ValueCounter::ValueCounter(size_t value, unsigned count) :
    pair(value, count)
{
}

ValueCounter &ValueCounter::operator +=(unsigned n)
{
    second += n;
    return *this;
}
