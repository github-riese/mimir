#ifndef ABSTRACTNEURON_H
#define ABSTRACTNEURON_H

#include "AbstractInput.h"
#include "AbstractBias.h"

namespace mimir {
namespace models {
namespace neurons {

class AbstractNeuron
{
public:
    AbstractNeuron();
    virtual ~AbstractNeuron();
    void virtual input(AbstractInput*, AbstractBias*) = 0;
};

} // namespace neurons
} // namespace models
} // namespace mimir
#endif // ABSTRACTNEURON_H
