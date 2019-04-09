#include "Sigmoid.h"

#include <cmath>

#include <helpers/math.h>

using namespace mimir::helpers::math;

namespace mimir {
namespace models {
namespace activation {

std::string Sigmoid::name() const
{
    return "sigmoid";
}

void Sigmoid::activate(std::vector<double> &v) const noexcept
{
    std::transform(v.begin(), v.end(), v.begin(), [](double v){
        return 1./(1.+std::exp(-v));
    });
}

std::vector<double> Sigmoid::derivative(const std::vector<double> &hypothesis) const noexcept
{
    auto result = hypothesis;
    apply(result, [](double sigmoid) { return sigmoid * (1 - sigmoid);});
    return result;
}

} // namespace activation
} // namespace models
} // namespace mimir
