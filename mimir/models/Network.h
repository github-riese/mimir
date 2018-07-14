#ifndef NETWORK_H
#define NETWORK_H

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
    std::vector<NetworkFragment> fragments() const;
private:
private:
    std::vector<NetworkFragment> _fragments;
};

} // namespace models
} // namespace mimir

#endif // NETWORK_H
