#ifndef PROBABILITY_H
#define PROBABILITY_H

#include <cmath>
#include <vector>

#include "ValueIndex.h"

namespace mimir {
namespace models {

class Probability
{
public:
    explicit Probability() {}
    inline explicit Probability(long double probability,
                                unsigned long totalSamples,
                                unsigned long totalInClass,
                                unsigned long totalInValue,
                                unsigned long countInClassAndValue,
                                std::vector<ValueIndex> samplers) :
        _probability(probability),
        _totalSamples(totalSamples),
        _totalInClass(totalInClass),
        _totalInValue(totalInValue),
        _countInClassAndValue(countInClassAndValue),
        _samplers(samplers)
    {}

    inline bool operator <(const Probability &rhs) { return _probability < rhs._probability; }
    inline bool operator ==(const Probability &rhs) { return _probability == rhs._probability; }
    inline operator bool() const { return valid(); }
    inline bool operator !() const { return !valid(); }
    inline bool valid() const { return _probability != NAN; }

    inline long double probability() const { return _probability; }


    inline unsigned long totalSamples() const { return _totalSamples; }

    inline unsigned long totalInClass() const { return _totalInClass; }

    inline unsigned long totalInValue() const { return _totalInValue; }

    inline unsigned long countInClassAndValue() const { return _countInClassAndValue; }

    inline std::vector<ValueIndex> samplers() const { return _samplers; }

private:
    long double _probability = NAN;
    unsigned long _totalSamples = 0L;
    unsigned long _totalInClass = 0L;
    unsigned long _totalInValue = 0L;
    unsigned long _countInClassAndValue = 0L;
    std::vector<ValueIndex> _samplers;
};

} // namespace models
} // namespace mimir

#endif // PROBABILITY_H
