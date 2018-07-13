#ifndef PROBABILITY_H
#define PROBABILITY_H

#include <cmath>
#include <limits>
#include <vector>

#include "ValueIndex.h"

namespace mimir {
namespace models {

class Probability
{
public:
    explicit inline Probability() : _probability(0) {}
    inline Probability(long double probability) :
        _probability(probability)
    {}

    inline Probability operator*(const Probability &rhs) { return _probability * rhs._probability; }
    inline Probability operator/(const Probability &rhs) { if (rhs.isZero()) return Probability{0}; return _probability / rhs._probability; }
    inline Probability operator+(const Probability &rhs) { return _probability + rhs._probability; }
    inline Probability &operator *=(const Probability &rhs) { _probability *= rhs._probability; return  *this; }
    inline Probability &operator /=(const Probability &rhs) { if(rhs.isZero()) _probability = 0.l; else  _probability /= rhs._probability; return  *this; }
    inline Probability &operator +=(const Probability &rhs) { _probability += rhs._probability; return  *this; }

    inline constexpr operator long double() const {return _probability; }
    inline constexpr bool operator <(const Probability &rhs) const { return _probability < rhs._probability; }
    inline bool operator ==(const Probability &rhs) const { return std::fabsl(_probability - rhs._probability) * 1e13L < std::min(std::fabsl(_probability), std::fabsl(rhs._probability)); }

    inline bool operator !() const { return !valid(); }


    inline bool isZero() const { return std::fabsl(_probability) * 1e10L < 1.L; }
    inline bool valid() const { return !std::isnan(_probability); }

    inline constexpr long double value() const { return _probability; }

private:
    long double _probability = std::numeric_limits<long double>::quiet_NaN();
};

} // namespace models
} // namespace mimir

inline mimir::models::Probability operator""_p(long double p) { return mimir::models::Probability(p); }
#endif // PROBABILITY_H
