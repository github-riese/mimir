#include "AbstractMSELossActivator.h"

#include <mimir/helpers/math.h>

namespace mimir {
namespace models {
namespace activation {

double AbstractMSELossActivator::error(const mimir::models::TrainerValueHelper &item) const noexcept
{
    auto diff = item.hypothesis() - item.expectation();
    diff *= diff;
    return  std::accumulate(diff.begin(), diff.end(), 0.)/static_cast<double>(diff.size());
}

std::vector<double> AbstractMSELossActivator::lossDerivative(const std::vector<double> &hypothesis, const std::vector<double> &expectations) const noexcept
{
    return (hypothesis - expectations) * derivative(hypothesis);
}

} // namespace activation
} // namespace models
} // namespace mimir
