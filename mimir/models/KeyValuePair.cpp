#include "KeyValuePair.h"
#include <algorithm>

template<typename Contained>
struct SortAsc
{
    bool operator()(Contained const &left, Contained const &right) const
    {
        return left < right;
    }
};

template<typename Contained>
struct SortDesc
{
    bool operator()(Contained const &left, Contained const &right) const
    {
        return right < left;
    }
};

void mimir::models::sort(mimir::models::ColumnNameValuePairVector &vector, bool ascending)
{
    if(ascending) {
        std::sort(vector.begin(), vector.end(), SortAsc<mimir::models::ColumnNameValuePair>());
    } else {
        std::sort(vector.begin(), vector.end(), SortDesc<mimir::models::ColumnNameValuePair>());
    }
}

void mimir::models::sort(mimir::models::ColumnIndexValuePairVector &vector, bool ascending)
{
    if (ascending)
        std::sort(vector.begin(), vector.end(), SortAsc<ColumnIndexValuePair>());
    else
        std::sort(vector.begin(), vector.end(), SortDesc<ColumnIndexValuePair>());
}
