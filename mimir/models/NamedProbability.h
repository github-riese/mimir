#ifndef NAMEDPROBABILITY_H
#define NAMEDPROBABILITY_H

#include <vector>
#include "models/Probability.h"
#include "models/ValueIndex.h"

namespace mimir {
namespace models {

struct ValueProbability {
    ValueIndex value;
    Probability probability;
    constexpr bool operator==(ValueProbability const &) const noexcept;
    constexpr bool operator<(ValueProbability const &) const noexcept;
    constexpr bool operator<(Probability const &) const noexcept;
};

using ValueProbabilityVector = std::vector<ValueProbability>;

constexpr bool ValueProbability::operator==(const ValueProbability &rhs) const noexcept
{
    return value == rhs.value && probability == rhs.probability;
}

constexpr bool ValueProbability::operator<(const ValueProbability &rhs) const noexcept
{
    return probability < rhs.probability;
}

constexpr bool ValueProbability::operator<(const Probability &p) const noexcept
{
    return probability < p;
}

}
}
#endif // NAMEDPROBABILITY_H
