#ifndef INTERNALNODETREE_H
#define INTERNALNODETREE_H

#include <vector>

#include <mimir/models/KeyValuePair.h>
#include <mimir/models/Probability.h>
#include <mimir/models/BayesNetFragment.h>
#include <mimir/models/CPT.h>

namespace mimir {
namespace models {
namespace detect {

struct InternalNode {
    ColumnIndexValuePair column;
    Probability probability;
    constexpr bool operator<(InternalNode const&rhs) const;
    constexpr operator bool() const;
};

using InternalNodeVector = std::vector<InternalNode>;

struct InternalFragment
{
    InternalNode node;
    std::vector<InternalFragment> parents;
    /**
     * @brief checks whether a fragment can be added as a parent of this fragment
     * as the resulting graph must be acyclic we check that none of the candidate's parents
     * are children down our paths
     * @param candidate
     * @return
     */
    bool canAdd(InternalFragment const &candidate) const;
};

using InternalFragmentVector = std::vector<InternalFragment>;

struct InternalNet
{
    InternalFragmentVector parents;
    bool canAdd(InternalFragment const &candidate) const;
};

BayesNetFragment convert(InternalFragment const &fragment, CPT const &cpt);

BayesNet convert(const InternalNet &inNet, ProbabilityDistribution const &classification, CPT const &cpt);

}
}
}
#endif // INTERNALNODETREE_H
