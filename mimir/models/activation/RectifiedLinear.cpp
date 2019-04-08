#include "RectifiedLinear.h"
#include <helpers/math.h>
#include <models/AbstractLayer.h>

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

std::vector<double> RectifiedLinear::activationPrime(const std::vector<double> &hypothesis) const noexcept
{
    auto derivative = hypothesis;
    apply(derivative, [](double x) { return x > 0. ? 1. : 0.;});
    return derivative;
}

std::vector<double> RectifiedLinear::activationPrime(const std::vector<double> &hypothesis, const std::vector<double> &) const noexcept
{
    return activationPrime(hypothesis);
}


} // namespace activation
} // namespace models
} // namespace mimir
