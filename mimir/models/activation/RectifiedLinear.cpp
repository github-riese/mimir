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
    std::transform(v.begin(), v.end(), v.begin(), [] (double v) -> double { return v > 0. ? v : .1 * v; });
}

std::vector<double> RectifiedLinear::derivative(const std::vector<double> &hypothesis) const noexcept
{
    auto derivative = hypothesis;
    // let's make that leaky relu
    apply(derivative, [](double x) { return x > 0. ? 1. : 0.1; });
    return derivative;
}


} // namespace activation
} // namespace models
} // namespace mimir
