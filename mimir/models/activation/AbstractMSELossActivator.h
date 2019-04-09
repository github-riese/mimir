#ifndef ABSTRACTMSELOSSACTIVATOR_H
#define ABSTRACTMSELOSSACTIVATOR_H

#include <mimir/models/activation/ActivationInterface.h>

namespace mimir {
namespace models {
namespace activation {

class AbstractMSELossActivator : public ActivationInterface
{
public:
    double error(mimir::models::TrainerValueHelper const &) const noexcept override;
    std::vector<double> lossDerivative(std::vector<double> const &hypothesis, std::vector<double> const &expectations) const noexcept override;
};

} // namespace activation
} // namespace models
} // namespace mimir

#endif // ABSTRACTMSELOSSACTIVATOR_H
