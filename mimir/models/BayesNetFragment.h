#ifndef NETWORKFRAGMENT_H
#define NETWORKFRAGMENT_H

#include <vector>

#include <iotaomegapsi/tools/logger/Logger.h>

#include "KeyValuePair.h"
#include "Probability.h"
#include "ProbabilityDistribution.h"

#include "../services/NameResolver.h"

namespace mimir {
namespace models {

struct Node {
    ColumnNameValuePair field;
    Probability probability;
    constexpr bool operator<(Node const &rhs) const
    {
        return probability < rhs.probability;
    }
    constexpr operator bool() const
    {
        return field.valid();
    }
};

struct BayesNetFragment {
    Node node;
    std::vector<BayesNetFragment> parents;
    size_t depth() const
    {
        if (parents.empty())
            return 1;
        size_t depth = 1;
        for (auto &parent : parents) {
            depth = std::max(depth, parent.depth());
        }
        return depth;
    }
};

struct BayesNet {
    ProbabilityDistribution classification;
    std::vector<BayesNetFragment> parents;
    size_t greatestDepth() const
    {
        if (parents.empty()) {
            return 0;
        }

        size_t depth = 0;
        for(auto parent : parents) {
            auto pdepth = parent.depth();
            depth = std::max(depth, pdepth);
        }
        return depth;
    }
};

using NodeVector = std::vector<Node>;

/*struct BayesNetFragment
{
    Node node;
    std::vector<Node> parents;

    inline bool operator <(BayesNetFragment const &rhs) const {
        if (node < rhs.node) {
            return true;
        }
        return parents.size() < rhs.parents.size();
    }
    inline iotaomegapsi::tools::logger::Logger &dump(iotaomegapsi::tools::logger::Logger &logger, services::NameResolver &nr) {
        auto stream = logger.info();
        if (!node) {
            stream << "(invalid BayesNetFragment: node is invalid.)";
            return logger;
        }
        stream << "P(";
        stream << nr.nameFromIndex(node.field.columnName) << "=" << nr.nameFromIndex(node.field.value);
        if (parents.size() > 0) {
            stream << "|";
            for (auto parent = parents.begin(); parent != parents.end(); ++ parent) {
                stream << nr.nameFromIndex(parent->field.columnName) << "=" << nr.nameFromIndex(parent->field.value);
                if (std::distance(parents.begin(), parent) < static_cast<decltype (std::distance(parents.begin(), parent))>(parents.size() - 1)) {
                    stream << ", ";
                }
            }
        }
        stream << ") = ";
        stream << node.probability;
        stream << "\n";
        return logger;
    }

    inline bool operator == (BayesNetFragment const &rhs) const {
        return node == rhs.node && parents == rhs.parents;
    }

    inline bool operator != (BayesNetFragment const &rhs) const {
        return node != rhs.node || parents != rhs.parents;
    }

};*/

using BayesNetFragmentVector = std::vector<BayesNetFragment>;


} // namespace models
} // namespace mimir

#endif // NETWORKFRAGMENT_H
