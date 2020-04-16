#ifndef NETBUILDER_H
#define NETBUILDER_H

#include <map>
#include <vector>

#include "models/KeyValuePair.h"
#include "models/detect/InternalNodeTree.h"

namespace mimir {
namespace services {
namespace detect {

class Netbuilder
{
public:
    Netbuilder(models::ColumnIndexValuePairVector const &fields, models::detect::InternalFragmentVector const &fragments);
    models::detect::InternalNet buildNet();
private:
    using ColumnIndex = long int;
    models::detect::InternalFragmentVector const &_fragments;
    std::map<ColumnIndex, int> _pool;
    std::map<ColumnIndex, int> _used;
};

} // namespace detect
} // namespace services
} // namespace mimir

#endif // NETBUILDER_H
