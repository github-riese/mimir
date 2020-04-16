#ifndef ABSTRACTMSELOSSACTIVATOR_H
#define ABSTRACTMSELOSSACTIVATOR_H

#include <mimir/models/activation/ActivationInterface.h>

namespace mimir {
namespace models {
namespace activation {

class AbstractMSELossActivator : public ActivationInterface
{
public:
    float error(mimir::models::TrainerValueHelper const &) const noexcept override;
    std::vector<float> lossDerivative(std::vector<float> const &hypothesis, std::vector<float> const &expectations) const noexcept override;
};

} // namespace activation
} // namespace models
} // namespace mimir

#endif // ABSTRACTMSELOSSACTIVATOR_H
