#ifndef NAMERESOLVESERIALIZER_H
#define NAMERESOLVESERIALIZER_H

#include <ios>
#include "NameResolver.h"

namespace mimir {
namespace services {

class NameResolveSerializer
{
public:
    void serialize(std::ostream &, NameResolver const &) const;
    NameResolver deserialize(std::istream &) const;
private:
    bool checkMagic(std::istream &) const;
};

} // namespace services
} // namespace mimir

#endif // NAMERESOLVESERIALIZER_H
