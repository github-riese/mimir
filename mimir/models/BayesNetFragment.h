#ifndef NETWORKFRAGMENT_H
#define NETWORKFRAGMENT_H

#include <vector>
#include <iostream>

#include "KeyValuePair.h"
#include "Probability.h"

#include "../services/NameResolver.h"

namespace mimir {
namespace models {

struct Node {
    ColumnNameValuePair namedValue;
    Probability probability;
    inline bool operator <(Node const &rhs) const {
        if (long fieldDiff = static_cast<long>(namedValue.columnName - rhs.namedValue.columnName)) {
            return fieldDiff < 0;
        }
        return probability < rhs.probability;
    }

    inline std::ostream & dump(std::ostream &stream, services::NameResolver &nr) {
        stream << nr.nameFromIndex(namedValue.columnName) << "=" << nr.nameFromIndex(namedValue.value);
        return stream;
    }

    inline bool operator == (Node const &rhs) const {
        return namedValue == rhs.namedValue;
    }

    inline bool operator != (Node const &rhs) const {
        return !(namedValue == rhs.namedValue);
    }
};

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
    inline std::ostream &dump(std::ostream &stream, services::NameResolver &nr) {
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
        stream << std::endl;
        return stream;
    }

    inline bool operator == (BayesNetFragment const &rhs) const {
        return node == rhs.node && parents == rhs.parents;
    }

    inline bool operator != (BayesNetFragment const &rhs) const {
        return node != rhs.node || parents != rhs.parents;
    }

};

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
