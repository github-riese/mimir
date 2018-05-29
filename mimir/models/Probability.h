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
    explicit Probability() {}
    inline explicit Probability(long double probability,
                                long double classProbability,
                                long double valueProbability,
                                std::vector<std::vector<ValueIndex>> samplers) :
        _probability(probability),
        _classProbability(classProbability),
        _valueProbability(valueProbability),
        _samplers(samplers)
    {}

    inline bool operator <(const Probability &rhs) const { return _probability < rhs._probability; }
    inline bool operator ==(const Probability &rhs) const { return std::fabsl(_probability - rhs._probability) * 1e13L < std::min(std::fabsl(_probability), std::fabsl(rhs._probability)); }

    inline bool isZero() const { return std::fabsl(_probability) * 1e10L < 1.L; }

    inline operator bool() const { return valid(); }
    inline bool operator !() const { return !valid(); }
    inline bool valid() const { return !std::isnan(_probability); }

    inline long double probability() const { return _probability; }

    inline long double classProbability() const { return _classProbability; }

    inline long double valueProbability() const { return _valueProbability; }

    inline std::vector<std::vector<ValueIndex>> samplers() const { return _samplers; }

private:
    long double _probability = std::numeric_limits<long double>::quiet_NaN();
    long double _classProbability = std::numeric_limits<long double>::quiet_NaN();
    long double _valueProbability = std::numeric_limits<long double>::quiet_NaN();
    std::vector<std::vector<ValueIndex>> _samplers;
};

} // namespace models
} // namespace mimir

#endif // PROBABILITY_H
