#ifndef VALUECOUNTER_H
#define VALUECOUNTER_H

#include <vector>
#include <utility>

#include "ValueIndex.h"

namespace mimir {
namespace models {

class ValueCounter
{
public:
    explicit ValueCounter();
    explicit ValueCounter(ValueIndex valueIndex, unsigned long count);
    inline ValueIndex valueIndex() const { return _valueIndex; }
    inline unsigned count() const { return _count; }
    inline ValueCounter &operator +=(unsigned long add) { _count += add; return *this; }
    inline void operator +(unsigned long add) { _count += add; }
    inline operator unsigned() const { return _count; }
private:
    ValueIndex _valueIndex;
    unsigned long _count;
};

} // namespace models
} // namespace mimir

#endif // VALUECOUNTER_H
