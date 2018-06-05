#ifndef ABSTRACTACTIVATION_H
#define ABSTRACTACTIVATION_H

namespace mimir {
namespace models {
namespace neurons {

struct AbstractActivation
{
    virtual ~AbstractActivation() {}
    virtual bool operator()() const = 0;
};

}
}
}
#endif // ABSTRACTACTIVATION_H
