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
    NameResolver();
    std::string nameFromIndex(const mimir::models::ValueIndex &) const;
    mimir::models::ValueIndex indexFromName(const std::string &);
    mimir::models::ValueIndex indexFromName(const std::string &) const;
    std::vector<std::string> const &wordList() const;
    void setWordList(std::vector<std::string> const &);
private:
    std::vector<std::string> _lookup;
};

} // namespace services
} // namespace mimir

#endif // NAMERESOLVER_H
