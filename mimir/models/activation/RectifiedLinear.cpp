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

std::vector<double> RectifiedLinear::biasGradient(std::vector<double> const &biases, std::vector<double> const &costDerivative) const noexcept
{
}

Matrix RectifiedLinear::weightGradient(const Matrix &weights, const std::vector<double> &costDerivative) const noexcept
{

}

std::vector<double> RectifiedLinear::delta(const std::vector<double> &zValues, const std::vector<double> &costDerivative) const noexcept
{
    auto result(zValues);
    std::transform(result.begin(), result.end(), result.begin(), [] (double v) -> double { return v < 0. ? 0. : 1.; });
    return result *= costDerivative;
}

} // namespace activation
} // namespace models
} // namespace mimir
