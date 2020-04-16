#include "Netbuilder.h"

namespace mimir {
namespace services {
namespace detect {

using models::BayesNet;
using models::ColumnIndexValuePairVector;
using models::ColumnNameValuePairVector;
using models::CPT;

using models::detect::InternalFragmentVector;
using models::detect::InternalNet;

Netbuilder::Netbuilder(const ColumnIndexValuePairVector &fields, const InternalFragmentVector &fragments) :
    _fragments(fragments)
{
    for(auto const &field : fields)
        _used[field.columnIndex] = 0;
    int n = 0;
    for (auto const &fragment : fragments) {
        _pool[fragment.node.column.columnIndex] = n;
        ++n;
    }
}

InternalNet Netbuilder::buildNet()
{
    InternalNet net;
    for (auto const &f : _fragments) {
        if (net.canAdd(f)) {
            net.parents.push_back(f);
            ++_used[f.node.column.columnIndex];
            for(auto const &p : f.parents)
                ++_used[p.node.column.columnIndex];

        }
    }
    return net;
}


} // namespace detect
} // namespace services
} // namespace mimir
