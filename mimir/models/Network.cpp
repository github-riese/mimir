#include "Network.h"

#include <algorithm>

using std::find_if;

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
    _sorted = false;
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

bool Network::canAdd(const NetworkFragment &fragment) const
{
    ValueIndex columnNameOfNew = fragment.input().columnName;
    for (auto f : _fragments) {
        if (f.input().columnName == columnNameOfNew) {
            return false;
        }
        if (f.hasParent(columnNameOfNew)) {
            return false;
        }
    }
    return true;
}

vector<NetworkFragment> Network::fragments() const
{
    if (!_sorted) {
        sort(const_cast<Network*>(this)->_fragments.begin(), const_cast<Network*>(this)->_fragments.end(),
             [](NetworkFragment const &left, NetworkFragment const &right) {
            if (left.input().columnName < right.input().columnName) {
                return true;
            }
            if (left.input().columnName == right.input().columnName) {
                return left.probability() > right.probability();
            }
            return false;
        });
        _sorted = true;
    }
    return _fragments;
}

int Network::greatestDepth() const
{
    int maxDepth = 0;
    for (auto f : _fragments) {
        int currentDepth = depthOf(f, 0);
        maxDepth = std::max(maxDepth, currentDepth);
    }
    return maxDepth;
}

std::vector<NamedProbability> Network::sinks() const
{
    vector<NamedProbability> result;
    for (auto f : _fragments) {
        if (isLeaf(f)) {
            result.push_back({f.input().columnName, f.probability()});
        }
    }
    return result;
}

std::ostream &Network::dump(std::ostream &stream, services::NameResolver &nr)
{
    for (auto f : _fragments)
        f.dump(stream, nr);
    return stream;
}


bool Network::isCyclic(const NetworkFragment &thisFragment, const NetworkFragment &base)
{
    for (auto parent : thisFragment.parents()) {
        if (parent.columnName == base.input().columnName) {
            return true;
        }
    }
    return false;
}

bool Network::isLeaf(const NetworkFragment &f) const
{
    if (f.countParents() == 0)
        return true;
    for ( auto fragment : _fragments) {
        if (fragment.hasParent(f.input().columnName)) {
            return false;
        }
    }
    return true;
}

int Network::depthOf(NetworkFragment &f, int depthBefore) const
{
    if (f.parents().size() == 0) {
        return depthBefore;
    }
    int currentDepth = depthBefore;
    for (auto p : f.parents()) {
        auto child = fragmentByChildFieldName(p.columnName);
        if (!child) {
            continue;
        }
        currentDepth = std::max(currentDepth, depthOf(*child, currentDepth+1));
    }
    return currentDepth;
}

std::experimental::optional<NetworkFragment> Network::fragmentByChildFieldName(ValueIndex name) const
{
    auto thing = find_if(_fragments.begin(), _fragments.end(), [name](NetworkFragment f) {
            return f.input().columnName == name;
    });
    if (thing == _fragments.end()) {
        return std::experimental::optional<NetworkFragment>();
    }
    return std::experimental::optional<NetworkFragment>(*thing);
}

} // namespace models
} // namespace mimir
