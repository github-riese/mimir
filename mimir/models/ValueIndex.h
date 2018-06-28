#ifndef VALUEINDEX_H
#define VALUEINDEX_H

#include <cstdlib>

namespace mimir {
namespace models {

class ValueIndex
{
public:
    inline ValueIndex() : _idx(NoIndex) {}
    static const size_t NoIndex = -1ul;
    static const size_t AnyIndex = -2ul;
    inline bool isValid() const { return _idx != NoIndex; }
    inline explicit ValueIndex(size_t idx) : _idx(idx) {}
    inline bool operator  <(const ValueIndex &rhs) const { return _idx < rhs._idx; }
    inline bool operator <=(const ValueIndex &rhs) const { return _idx <= rhs._idx; }
    inline bool operator ==(const ValueIndex &rhs) const { return _idx == rhs._idx; }
    inline constexpr operator size_t() const { return _idx; }
    inline constexpr size_t value() const { return _idx; }
private:
    size_t _idx;
};

inline bool operator <= (size_t s, ValueIndex i) { return ValueIndex(s) <= i; }

} // namespace models
} // namespace mimir
inline mimir::models::ValueIndex operator "" _vi(unsigned long long i) { return mimir::models::ValueIndex((size_t)i); }

#endif // VALUEINDEX_H
