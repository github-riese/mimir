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
        if ((*fragment).input().columnName == column) {
            return true;
        }
        ++fragment;
    }
    return false;
}

bool Network::canAdd(const NetworkFragment &fragment) const
{
    if (fragment.countParents() == 0) {
        return true;
    }

    ValueIndex columnNameOfNew = fragment.input().columnName;
    for (auto f : _fragments) {
        if (f.hasParent(columnNameOfNew) || fragment.hasParent(f.input().columnName)) {
            return false;
        }
    }
    return true;
}

vector<NetworkFragment> Network::fragments() const
{
    sort();
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

std::vector<NamedProbability> Network::sinks(vector<ValueIndex> const &availableFieldNames) const
{
    vector<NamedProbability> result;
    vector<std::pair<ValueIndex, int>> seenFields;
    for (auto i = availableFieldNames.begin(); i != availableFieldNames.end(); ++i) {
        seenFields.push_back({*i, 0});
    }
    for (auto f : _fragments) {
        if (isLeaf(f)) {
            result.push_back({f.input().columnName, f.probability()});
            for (auto p : f.parents()) {
                auto i = find_if(seenFields.begin(), seenFields.end(), [p](std::pair<ValueIndex, int> const&v){
                        return v.first == p.columnName;
            });
                if (i != seenFields.end()) {
                    (*i).second += 1;
                }
            }
        }
    }
    for (auto seen : seenFields) {
        if (seen.second == 0) {
//            result.push_back({seen.first});
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
    auto foundChildByName = find_if(_fragments.begin(), _fragments.end(), [name](NetworkFragment f) {
            return f.input().columnName == name;
    });
    if (foundChildByName == _fragments.end()) {
        return std::experimental::optional<NetworkFragment>();
    }
    return std::experimental::optional<NetworkFragment>(*foundChildByName);
}

void Network::sort() const
{
    if (_sorted)
        return;
        std::sort(const_cast<Network*>(this)->_fragments.begin(), const_cast<Network*>(this)->_fragments.end(),
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

} // namespace models
} // namespace mimir
