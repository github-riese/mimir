#ifndef ABSTRACTBIAS_H
#define ABSTRACTBIAS_H

#include "AbstractInput.h"

namespace mimir {
namespace models {
namespace neurons {

class AbstractBias
{
public:
    AbstractBias();
    virtual ~AbstractBias();
    virtual AbstractInput *bias() = 0;
};

} // namespace neurons
} // namespace models
} // namespace mimir

#endif // ABSTRACTBIAS_H
