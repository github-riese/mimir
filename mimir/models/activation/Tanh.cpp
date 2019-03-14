#include "Tanh.h"

#include <cmath>

#include "../../helpers/math.h"

namespace mimir {
namespace models {
namespace activation {

std::string Tanh::name() const
{
    return "tanh";
}

void Tanh::activate(std::vector<double> &v) const noexcept
{
    std::transform(v.begin(), v.end(), v.begin(), [] (double v) -> double {
        return std::tanh(v);
    });
}

void Tanh::derivative(std::vector<double> &v) const noexcept
{
    activate(v);
    std::transform(v.begin(), v.end(), v.begin(), [](double v) -> double {
        return 1/(v*v);
    });
}

std::vector<double> Tanh::loss(const std::vector<double> &hypothesis, const std::vector<double> &expectation) const noexcept
{
    return expectation - hypothesis;
}


} // namespace activation
} // namespace models
} // namespace mimir
