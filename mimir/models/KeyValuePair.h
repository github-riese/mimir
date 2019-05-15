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
    inline constexpr bool valid() const
    {
        return columnName.isValid() && value.isValid();
    }

    constexpr inline bool operator <(ColumnNameValuePair const &rhs) const
    {
        return columnName < rhs.columnName;
    }

    constexpr inline bool operator ==(ColumnNameValuePair const &rhs) const
    {
        return columnName == rhs.columnName && value == rhs.value;
    }
};

using ColumnNameValuePairVector = std::vector<ColumnNameValuePair>;

struct ColumnIndexValuePair
{
    long int columnIndex;
    ValueIndex value;
    constexpr inline bool operator<(ColumnIndexValuePair const &rhs) const
    {
        return columnIndex < rhs.columnIndex;
    }

    bool operator==(ColumnIndexValuePair const &rhs) const {
        return rhs.columnIndex == columnIndex && rhs.value == rhs.value;
    }
};

using ColumnIndexValuePairVector = std::vector<ColumnIndexValuePair>;

/**
 * @brief The ConditionalProbability struct
 * for expressions P(k|x1, x2, ..., xn) k is in name, X is in parents
 */
struct ConditionalProbability
{
    Probability probability;
    ValueIndex name;
    std::vector<ValueIndex> parents;
    constexpr inline bool operator <(ConditionalProbability const &rhs) const noexcept
    {
        return probability < rhs.probability;
    }
    constexpr inline bool operator==(ConditionalProbability const &rhs) const noexcept
    {
        return probability == rhs.probability;
    }
};

} // namespace models
} // namespace mimir

#endif // KEYVALUEPAIR_H
