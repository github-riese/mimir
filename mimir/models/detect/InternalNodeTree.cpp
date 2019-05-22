#include "InternalNodeTree.h"
#include "../BayesNetFragment.h"

namespace mimir {
namespace models {
namespace detect {

constexpr bool InternalNode::operator<(const InternalNode &rhs) const
{
    return  probability < rhs.probability;
}

constexpr InternalNode::operator bool() const
{
    return probability.valid();
}

BayesNetFragment convert(const InternalFragment &fragment, const CPT &cpt)
{
    BayesNetFragment outFragment;
    outFragment.node = {{cpt.fieldName(fragment.node.column.columnIndex), fragment.node.column.value},
                        fragment.node.probability};
    for(auto p : fragment.parents) {
        outFragment.parents.push_back(convert(p, cpt));
    }
    return outFragment;
}

BayesNet convert(const InternalNet &inNet, const ProbabilityDistribution &classification, const CPT &cpt)
{
    BayesNet net;
    net.classification = classification;
    for (auto p : inNet.parents) {
        net.parents.push_back(convert(p, cpt));
    }
    return net;
}

bool InternalFragment::canAdd(const InternalFragment &candidate) const
{
    for (auto p : candidate.parents) {
        if (p.node.column.columnIndex == node.column.columnIndex)
            return false;
    }
    for (auto p : parents) {
        if (!p.canAdd(candidate))
            return false;
    }
    return true;
}

bool InternalNet::canAdd(const InternalFragment &candidate) const
{
    for (auto p : parents) {
        if (!p.canAdd(candidate))
            return false;
    }
    return true;
}

}
}
}
