#ifndef NETWORKFRAGMENT_H
#define NETWORKFRAGMENT_H

#include <vector>

#include <iotaomegapsi/tools/logger/Logger.h>

#include "KeyValuePair.h"
#include "Probability.h"

#include "../services/NameResolver.h"

namespace mimir {
namespace models {

struct Node {
    ColumnNameValuePair namedValue;
    Probability probability;

    inline constexpr bool valid() const
    {
        return namedValue.valid();
    }

    inline bool operator <(Node const &rhs) const {
        if (long fieldDiff = static_cast<long>(namedValue.columnName - rhs.namedValue.columnName)) {
            return fieldDiff < 0;
        }
        return probability < rhs.probability;
    }

    inline iotaomegapsi::tools::logger::LogMessage dump(iotaomegapsi::tools::logger::LogMessage &message, services::NameResolver &nr) {
        message << nr.nameFromIndex(namedValue.columnName) << "=" << nr.nameFromIndex(namedValue.value);
        return message;
    }

    inline bool operator == (Node const &rhs) const {
        return namedValue == rhs.namedValue;
    }

    inline bool operator != (Node const &rhs) const {
        return !(namedValue == rhs.namedValue);
    }
};

using NodeVector = std::vector<Node>;

struct BayesNetFragment
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
        if (!node.valid()) {
            stream << "(invalid BayesNetFragment: node is invalid.)";
            return logger;
        }
        stream << "P(";
        node.dump(stream, nr);
        if (parents.size() > 0) {
            stream << "|";
            for (auto parent = parents.begin(); parent != parents.end(); ++ parent) {
                (*parent).dump(stream, nr);
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

};

using BayesNetFragmentVector = std::vector<BayesNetFragment>;


} // namespace models
} // namespace mimir

#endif // NETWORKFRAGMENT_H
