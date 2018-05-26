#ifndef NAMERESOLVER_H
#define NAMERESOLVER_H

#include <map>
#include <vector>
#include <string>

#include "../models/ValueIndex.h"

namespace mimir {
namespace services {

class NameResolver
{
public:
    enum class NameSource
    {
        Classification = 0U,
        Property = 1U,
        Value = 2U
    };
    NameResolver();
    std::string nameFromIndex(NameSource, const mimir::models::ValueIndex &) const;
    mimir::models::ValueIndex indexFromName(NameSource, const std::string &);
    mimir::models::ValueIndex indexFromName(NameSource, const std::string &) const;
private:
    mimir::models::ValueIndex index(NameSource, const std::string&) const;
private:
    std::map<NameSource, std::vector<std::string>> _lookup;
};

} // namespace services
} // namespace mimir

#endif // NAMERESOLVER_H
