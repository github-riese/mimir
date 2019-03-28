#include "RectifiedLinear.h"
#include <helpers/math.h>

namespace mimir {
namespace models {
namespace activation {

std::string RectifiedLinear::name() const
{
    return "rectifiedLinear";
}

void RectifiedLinear::activate(std::vector<double> &v) const noexcept
{
    std::transform(v.begin(), v.end(), v.begin(), [] (double v) -> double { return std::max(0., v); });
}

std::vector<double> RectifiedLinear::biasGradient(const std::vector<double> &hypothesis, const std::vector<double> &costDerivative) const noexcept
{
    auto hypothesisDerivative = hypothesis;
    derivative(hypothesisDerivative);
    return hypothesisDerivative *= costDerivative;
}

void RectifiedLinear::derivative(std::vector<double> &hypothesis) const noexcept
{
    std::transform(hypothesis.begin(), hypothesis.end(), hypothesis.begin(), [](double activated) noexcept -> double {
        return activated > 0. ? 1. : 0.;
    });
}

} // namespace activation
} // namespace models
} // namespace mimir
