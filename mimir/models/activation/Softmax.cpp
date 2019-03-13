#include "Softmax.h"
#include <numeric>
#include <cmath>
#include "../../helpers/math.h"

namespace mimir {
namespace models {
namespace activation {

std::string Softmax::name() const
{
    return "softmax";
}

void Softmax::activate(std::vector<double> &v) const noexcept
{
    auto maxV = *std::max_element(v.begin(), v.end());
    v -= maxV;
    auto sum = std::accumulate(v.begin(), v.end(), 0, [](double init, double v) noexcept -> double {
        return init + std::exp(v);
    });
    apply(v, [&sum](double v) noexcept (true) -> double { return std::exp(v) / sum; });
}

void Softmax::derivative(std::vector<double> &v) const noexcept
{

}

} // namespace activation
} // namespace models
} // namespace mimir
