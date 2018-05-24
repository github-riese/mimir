#ifndef VALUECOUNTER_H
#define VALUECOUNTER_H

#include <vector>
#include <utility>

using std::pair;

class ValueCounter : public pair<size_t, unsigned>
{
public:
    explicit ValueCounter();
    ValueCounter(size_t value, unsigned count = 1);
    inline size_t value() const { return first; }
    inline unsigned count() const { return second; }
    ValueCounter &operator +=(unsigned);
    inline operator unsigned() const { return second; }
};

#endif // VALUECOUNTER_H
