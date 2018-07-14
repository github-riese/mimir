#include "Network.h"

using std::vector;

namespace mimir {
namespace models {

Network::Network()
{

}

void Network::addFragment(const NetworkFragment &fragment)
{
    auto parents = fragment.parents();
    std::sort(parents.begin(), parents.end());
    NetworkFragment newFragment{fragment.input(), parents, fragment.probability()};
    for (auto f : _fragments) {
        if (f.input() == newFragment.input() &&
                f.parents() == parents) {
            return;
        }
    }
    _fragments.push_back(newFragment);
}

bool Network::isKnownChild(ValueIndex column) const
{
    auto fragment = _fragments.begin();
    while (fragment != _fragments.end()) {
        if (fragment->input().columnName == column) {
            return true;
        }
        ++fragment;
    }
    return false;
}

vector<NetworkFragment> Network::fragments() const
{
    return _fragments;
}

} // namespace models
} // namespace mimir
