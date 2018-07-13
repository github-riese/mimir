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

    ColumnNameValuePair value() const;

    std::vector<ColumnNameValuePair> parents() const;

    Probability probability() const;

    std::ostream &dump(std::ostream &, services::NameResolver &);
private:
    ColumnNameValuePair _value;
    std::vector<ColumnNameValuePair> _parents;
    Probability _probability;
};

} // namespace models
} // namespace mimir

#endif // NETWORKFRAGMENT_H
