#ifndef ABSTRACTMSELOSSACTIVATOR_H
#define ABSTRACTMSELOSSACTIVATOR_H

#include <mimir/models/activation/ActivationInterface.h>

namespace mimir {
namespace models {
namespace activation {

class AbstractMSELossActivator : public ActivationInterface
{
public:
    double loss(const std::vector<std::vector<double>> &results, const std::vector<std::vector<double>> &expectations) const noexcept override;
};

} // namespace activation
} // namespace models
} // namespace mimir

#endif // ABSTRACTMSELOSSACTIVATOR_H
