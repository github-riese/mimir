#include "NameResolver.h"

#include <algorithm>

using std::string;
using std::find_if;

using mimir::models::ValueIndex;
using mimir::models::InvalidIndex;

namespace mimir {
namespace services {

NameResolver::NameResolver()
{
}

string NameResolver::nameFromIndex(NameSource source, const ValueIndex &index) const
{
    auto theSet = _lookup.find(source);
    if (theSet == _lookup.end()) {
        return string();
    }
    if ((*theSet).second.size() <= index) {
        return string();
    }

    size_t i = index;
    auto iterator = (*theSet).second.begin();
    while (i--) {
        ++iterator;
    }
    return *iterator;
}

ValueIndex NameResolver::indexFromName(NameSource source, const string &value)
{
    ValueIndex idx = index(source, value);
    if (idx.isValid()) {
        return idx;
    }
    _lookup[source].push_back(value);
    return ValueIndex(_lookup[source].size() -1);
}

ValueIndex NameResolver::indexFromName(NameResolver::NameSource source, const std::string &value) const
{
    return index(source, value);
}

models::ValueIndex NameResolver::index(NameResolver::NameSource source, const std::string &value) const
{
    auto theSet = _lookup.find(source);
    if (theSet == _lookup.end()) {
        return InvalidIndex();
    }

    size_t index = 0;
    auto theIndex = find_if((*theSet).second.begin(), (*theSet).second.end(), [&value, &index](const string &current) {
        if (current == value) {
            return true;
        }
        ++index;
        return false;
    });
    if (theIndex == (*theSet).second.end()) {
        return InvalidIndex();
    }
    return ValueIndex(theIndex - (*theSet).second.begin());
}

} // namespace services
} // namespace mimir
