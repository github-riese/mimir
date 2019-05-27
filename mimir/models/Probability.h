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
    inline explicit Probability() : _probability(0.) {}
    inline Probability(double probability) :
        _probability(probability)
    {}

    inline Probability operator*(const Probability &rhs) noexcept { return _probability * rhs._probability; }
    inline Probability operator/(const Probability &rhs) noexcept { if (rhs.isZero()) return Probability{0}; return _probability / rhs._probability; }
    inline Probability operator+(const Probability &rhs) noexcept { return _probability + rhs._probability; }
    inline Probability &operator *=(const Probability &rhs) noexcept { _probability *= rhs._probability; return  *this; }
    inline Probability &operator /=(const Probability &rhs) noexcept { if(rhs.isZero()) _probability = 0.l; else  _probability /= rhs._probability; return  *this; }
    inline Probability &operator +=(const Probability &rhs) noexcept { _probability += rhs._probability; return  *this; }

    inline constexpr operator double() const noexcept { return _probability; }
    inline constexpr bool operator <(const Probability &rhs) const noexcept { return _probability < rhs._probability; }
    inline bool operator ==(const Probability &rhs) const noexcept { return std::fabs(_probability - rhs._probability) * 1e7 < std::min(std::fabs(_probability), std::fabs(rhs._probability)); }
    inline bool equals(const Probability &rhs, double precision) const noexcept { return  std::fabs(_probability - rhs._probability) * precision < std::min(std::fabs(_probability), std::fabs(rhs._probability)); }

    inline bool operator !() const noexcept { return !valid(); }


    inline bool isZero() const noexcept { return std::fabs(_probability) * 1e10 < 1.; }
    inline bool valid() const noexcept { return !std::isnan(_probability); }

    inline constexpr double value() const { return _probability; }

private:
    double _probability = std::numeric_limits<double>::quiet_NaN();
};

} // namespace models
} // namespace mimir

inline mimir::models::Probability operator""_p(long double p) { return mimir::models::Probability(static_cast<double>(p)); }
#endif // PROBABILITY_H
