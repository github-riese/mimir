#ifndef NAMEDPROBABILITY_H
#define NAMEDPROBABILITY_H

#include <vector>
#include "models/KeyValuePair.h"
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

struct NamedProbability
{
    ColumnIndexValuePair value;
    Probability probability;
    constexpr bool operator<(NamedProbability const &rhs) const noexcept;
};

using NamedProbabilityVector = std::vector<NamedProbability>;

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

constexpr bool NamedProbability::operator<(const NamedProbability &rhs) const noexcept
{
    return probability < rhs.probability;
}

}
}
#endif // NAMEDPROBABILITY_H
