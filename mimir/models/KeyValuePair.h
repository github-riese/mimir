#ifndef KEYVALUEPAIR_H
#define KEYVALUEPAIR_H

#include <vector>

#include "ValueIndex.h"
#include "Probability.h"

namespace mimir {
namespace models {

struct ColumnNameValuePair
{
    ValueIndex columnName;
    ValueIndex value;
    constexpr inline bool operator <(ColumnNameValuePair const &rhs) const
    {
        return columnName < rhs.columnName;
    }

    constexpr inline bool operator ==(ColumnNameValuePair const &rhs) const
    {
        return columnName == rhs.columnName && value == rhs.value;
    }
};

struct ColumnIndexValuePair
{
    long int columnIndex;
    ValueIndex value;
    constexpr inline bool operator<(ColumnIndexValuePair const &rhs) const
    {
        return columnIndex < rhs.columnIndex;
    }
};

struct NamedProbability
{
    ValueIndex name;
    Probability probability;
    constexpr inline bool operator <(NamedProbability const &rhs) const
    {
        return probability < rhs.probability;
    }

    constexpr inline bool operator ==(NamedProbability const &rhs) const
    {
        return probability == rhs.probability && name == rhs.name;
    }
};

/**
 * @brief The ConditionalProbability struct
 * for expressions P(k|x1, x2, ..., xn) k is in name, X is in parents
 */
struct ConditionalProbability
{
    Probability probability;
    ValueIndex name;
    std::vector<ValueIndex> parents;
    constexpr inline bool operator <(ConditionalProbability const &rhs) const
    {
        return probability < rhs.probability;
    }
    constexpr inline bool operator==(ConditionalProbability const &rhs) const
    {
        return probability == rhs.probability;
    }
};

} // namespace models
} // namespace mimir

#endif // KEYVALUEPAIR_H
