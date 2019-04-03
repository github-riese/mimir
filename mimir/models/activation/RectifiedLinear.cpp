#include "RectifiedLinear.h"
#include <helpers/math.h>
#include <models/Layer.h>

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

std::vector<double> RectifiedLinear::derivative(const std::vector<double> &zValues) const noexcept
{
    auto derivative = zValues;
    apply(derivative, [](double x) { return x > 0. ? 1. : 0.;});
    return derivative;
}


} // namespace activation
} // namespace models
} // namespace mimir
