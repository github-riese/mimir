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
};

struct ColumnIndexValuePair
{
    long int columnIndex;
    ValueIndex value;
};

struct ValueProbabilityPair
{
    ValueIndex name;
    Probability probability;
};

struct ValueCompare
{
    virtual ~ValueCompare();
    inline virtual bool operator ()(ValueIndex left, ValueIndex right) = 0;
};

struct Equals : public ValueCompare
{
    inline virtual bool operator ()(ValueIndex left, ValueIndex right)
    {
        return left == right;
    }
};

struct NotEquals : public ValueCompare
{
    inline virtual bool operator ()(ValueIndex left, ValueIndex right)
    {
        return left != right;
    }
};


} // namespace models
} // namespace mimir

#endif // KEYVALUEPAIR_H
