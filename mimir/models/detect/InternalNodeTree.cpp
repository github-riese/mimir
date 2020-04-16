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
    if (node.column.columnIndex == candidate.node.column.columnIndex)
        return false;
    for (auto p : candidate.parents) {
        if (p.node.column.columnIndex == node.column.columnIndex && parents.size() > 0)
            return false;
    }
    for (auto p : parents) {
        if (!p.canAdd(candidate))
            return false;
    }
    return true;
}

ColumnIndexValuePairVector InternalFragment::parentValues() const
{
    ColumnIndexValuePairVector parentValues;
    for(auto const &p : parents)
        parentValues.push_back(p.node.column);
    return parentValues;
}

bool InternalNet::canAdd(const InternalFragment &candidate) const
{
    for (auto p : parents) {
        if (!p.canAdd(candidate))
            return false;
    }
    return true;
}

bool mimir::models::detect::InternalFragment::contains(const ColumnIndexValuePair &search) const
{
    if (node.column == search)
        return true;
    for (auto p : parents)
        if (p.contains(search))
            return true;
    return false;
}

}
}
}
