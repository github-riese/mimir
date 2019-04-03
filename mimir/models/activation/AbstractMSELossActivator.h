#ifndef ABSTRACTMSELOSSACTIVATOR_H
#define ABSTRACTMSELOSSACTIVATOR_H

#include <mimir/models/activation/ActivationInterface.h>

namespace mimir {
namespace models {
namespace activation {

class AbstractMSELossActivator : public ActivationInterface
{
public:
    double loss(mimir::models::TrainerValueHelper const &) const noexcept override;
};

} // namespace activation
} // namespace models
} // namespace mimir

#endif // ABSTRACTMSELOSSACTIVATOR_H
