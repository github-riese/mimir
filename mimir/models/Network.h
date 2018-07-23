#ifndef NETWORK_H
#define NETWORK_H

#include <iostream>
#include <experimental/optional>
#include <vector>

#include "NetworkFragment.h"

namespace mimir {
namespace models {

class Network
{
public:
    Network();
    void addFragment(NetworkFragment const &fragment);
    bool isKnownChild(ValueIndex) const;
    bool canAdd(NetworkFragment const &fragment) const;
    std::vector<NetworkFragment> fragments() const;
    int greatestDepth() const;
    std::vector<NamedProbability> sinks() const;
    std::ostream &dump(std::ostream &, services::NameResolver &);
private:
    bool isCyclic(const NetworkFragment &, const NetworkFragment &);
    bool isLeaf(const NetworkFragment &) const;
    int depthOf(NetworkFragment &f, int depthBefore) const;
    std::experimental::optional<NetworkFragment> fragmentByChildFieldName(ValueIndex) const;
private:
    mutable bool _sorted = false;
    std::vector<NetworkFragment> _fragments;
};

} // namespace models
} // namespace mimir

#endif // NETWORK_H
