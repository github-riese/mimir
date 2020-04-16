#include "AbstractMSELossActivator.h"

#include <mimir/helpers/math.h>

namespace mimir {
namespace models {
namespace activation {

float AbstractMSELossActivator::error(const mimir::models::TrainerValueHelper &item) const noexcept
{
    auto diff = item.hypothesis() - item.expectation();
    diff *= diff;
    return  std::accumulate(diff.begin(), diff.end(), 0.)/static_cast<float>(diff.size());
}

std::vector<float> AbstractMSELossActivator::lossDerivative(const std::vector<float> &hypothesis, const std::vector<float> &expectations) const noexcept
{
    return (hypothesis - expectations) * derivative(hypothesis);
}

} // namespace activation
} // namespace models
} // namespace mimir
