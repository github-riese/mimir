#include "RectifiedLinear.h"

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

void RectifiedLinear::derivative(std::vector<double> &v) const noexcept
{
    std::transform(v.begin(), v.end(), v.begin(), [] (double v) -> double { return v < 0. ? 0. : 1.; });
}

} // namespace activation
} // namespace models
} // namespace mimir
