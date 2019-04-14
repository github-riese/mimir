#ifndef NETWORK_H
#define NETWORK_H

#include <iostream>
#include <optional>
#include <vector>

#include "BayesNetFragment.h"

namespace mimir {
namespace models {

class BayesNet
{
public:
    BayesNet();
    void addFragment(NetworkFragment const &fragment);
    bool isKnownChild(ValueIndex) const;
    bool canAdd(NetworkFragment const &fragment) const;
    std::vector<NetworkFragment> fragments() const;
    int greatestDepth() const;
    std::vector<NamedProbability> sinks(std::vector<ValueIndex> const &availableFieldNames) const;
    std::ostream &dump(std::ostream &, services::NameResolver &);
private:
    bool isCyclic(const NetworkFragment &, const NetworkFragment &);
    bool isLeaf(const NetworkFragment &) const;
    int depthOf(NetworkFragment &f, int depthBefore) const;
    std::optional<NetworkFragment> fragmentByChildFieldName(ValueIndex) const;
    void sort() const;
private:
    mutable bool _sorted = false;
    std::vector<NetworkFragment> _fragments;
};

} // namespace models
} // namespace mimir

#endif // NETWORK_H
