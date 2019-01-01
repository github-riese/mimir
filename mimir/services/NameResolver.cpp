#include "NameResolver.h"

#include <algorithm>

using std::string;
using std::find_if;

using mimir::models::ValueIndex;


namespace mimir {
namespace services {

NameResolver::NameResolver()
{
}

string NameResolver::nameFromIndex(const ValueIndex &index) const
{
    if (index >= _lookup.size()) {
        return string();
    }
    return _lookup.at(index);
}

ValueIndex NameResolver::indexFromName(const string &value)
{
    auto existing = find_if(_lookup.begin(), _lookup.end(), [value](const string &item){
        return item == value;
    });
    if (existing == _lookup.end()) {
        _lookup.push_back(value);
        return ValueIndex(_lookup.size() -1);
    }
    return ValueIndex(static_cast<size_t>(std::distance(_lookup.begin(), existing)));
}

ValueIndex NameResolver::indexFromName(const std::string &value) const
{
    auto existing = find_if(_lookup.begin(), _lookup.end(), [&value](const string &item) {
        return item == value;
    });
    if (existing == _lookup.end()) {
        return ValueIndex();
    }
    return ValueIndex(static_cast<size_t>(std::distance(_lookup.begin(), existing)));
}

const std::vector<std::string> &NameResolver::wordList() const
{
    return _lookup;
}

void NameResolver::setWordList(const std::vector<std::string> &words)
{
    _lookup = words;
}

} // namespace services
} // namespace mimir
