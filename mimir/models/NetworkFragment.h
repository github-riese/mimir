#ifndef NETWORKFRAGMENT_H
#define NETWORKFRAGMENT_H

#include <vector>
#include <iostream>

#include "KeyValuePair.h"
#include "Probability.h"

#include "../services/NameResolver.h"

namespace mimir {
namespace models {

class NetworkFragment
{
public:
    NetworkFragment(ColumnNameValuePair, std::vector<ColumnNameValuePair>, Probability);

    ColumnNameValuePair input() const;

    std::vector<ColumnNameValuePair> parents() const;

    Probability probability() const;

    inline bool operator <(NetworkFragment const &rhs) const
    {
        if (long tdiff = static_cast<long>(_input.columnName - rhs._input.columnName)) {
            return tdiff < 0;
        }
        if (_probability < rhs._probability) {
            return true;
        }
        return _parents.size() < rhs._parents.size();
    }

    inline size_t countParents() const
    {
        return _parents.size();
    }

    bool hasParent(ValueIndex) const;

    std::ostream &dump(std::ostream &, services::NameResolver &);
private:
    ColumnNameValuePair _input;
    std::vector<ColumnNameValuePair> _parents;
    Probability _probability;
};

} // namespace models
} // namespace mimir

#endif // NETWORKFRAGMENT_H
