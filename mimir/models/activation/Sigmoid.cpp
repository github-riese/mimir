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

void Sigmoid::activate(std::vector<float> &v) const noexcept
{
    std::transform(v.begin(), v.end(), v.begin(), [](float v){
        return 1./(1.+std::exp(-v));
    });
}

std::vector<float> Sigmoid::derivative(const std::vector<float> &hypothesis) const noexcept
{
    auto result = hypothesis;
    apply(result, [](float sigmoid) { return sigmoid * (1 - sigmoid);});
    return result;
}

} // namespace activation
} // namespace models
} // namespace mimir
