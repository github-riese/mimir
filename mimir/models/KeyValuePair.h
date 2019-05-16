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
        return columnName.valid() && value.valid();
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
    long int columnIndex = -1;
    ValueIndex value;

    constexpr inline bool valid() const
    {
        return columnIndex >= 0 && value.valid();
    }

    constexpr inline bool operator<(ColumnIndexValuePair const &rhs) const
    {
        return columnIndex < rhs.columnIndex;
    }

    bool operator==(ColumnIndexValuePair const &rhs) const {
        return rhs.columnIndex == columnIndex && rhs.value == rhs.value;
    }
};

using ColumnIndexValuePairVector = std::vector<ColumnIndexValuePair>;

} // namespace models
} // namespace mimir

#endif // KEYVALUEPAIR_H
