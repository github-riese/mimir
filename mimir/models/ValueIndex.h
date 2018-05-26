#ifndef VALUEINDEX_H
#define VALUEINDEX_H

#include <cstdlib>

namespace mimir {
namespace models {

class ValueIndex
{
public:
    static const size_t NoIndex = (size_t) -1;
    inline bool isValid() const { return _idx != NoIndex; }
    inline explicit ValueIndex(size_t idx) : _idx(idx) {}
    inline bool operator  <(const ValueIndex &rhs) const { return _idx < rhs._idx; }
    inline bool operator <=(const ValueIndex &rhs) const { return _idx <= rhs._idx; }
    inline bool operator ==(const ValueIndex &rhs) const { return _idx == rhs._idx; }
    inline operator size_t() const { return _idx; }
private:
    size_t _idx;
};

struct InvalidIndex : public ValueIndex
{
    inline explicit InvalidIndex() : ValueIndex(ValueIndex::NoIndex) {}
};

inline bool operator <= (size_t s, ValueIndex i) { return ValueIndex(s) <= i; }

} // namespace models
} // namespace mimir
inline mimir::models::ValueIndex operator "" _vi(unsigned long long i) { return mimir::models::ValueIndex((size_t)i); }

#endif // VALUEINDEX_H
