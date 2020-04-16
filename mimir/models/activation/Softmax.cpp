#include "Softmax.h"
#include <numeric>
#include <cmath>

#include <boost/range/combine.hpp>

#include <helpers/helpers.h>
#include <helpers/math.h>
#include <mimir/models/Matrix.h>

namespace mimir {
namespace models {
namespace activation {

std::string Softmax::name() const
{
    return "softmax";
}

void Softmax::activate(std::vector<float> &v) const noexcept
{
    auto maxV = *std::max_element(v.begin(), v.end());
    v -= maxV;
    auto sum = std::accumulate(v.begin(), v.end(), 0., [](float init, float v) noexcept -> float {
        return init + std::exp(v);
    });
    apply(v, [&sum](float v) noexcept (true) -> float { return std::exp(v) / sum; });
}

std::vector<float> Softmax::derivative(std::vector<float> const &hypothesis) const noexcept
{
    auto smax = hypothesis;
    return smax * (1.f - smax);
}

float Softmax::error(const TrainerValueHelper &values) const noexcept
{
    auto combined = boost::combine(values.hypothesis(), values.expectation());
    float crossEntropy = std::accumulate(combined.begin(), combined.end(), 0., [](float init, auto tuple) -> float {
        float hypothesis, expectation;
        boost::tie(hypothesis, expectation) = tuple;
        return init + (expectation * std::log(hypothesis));
    });
    return -crossEntropy;
}

std::vector<float> Softmax::lossDerivative(const std::vector<float> &hypothesis, const std::vector<float> &expectations) const noexcept
{
    std::vector<float> dLoss = expectations - hypothesis;
    return -dLoss;
}

} // namespace activation
} // namespace models
} // namespace mimir
