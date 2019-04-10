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

void Softmax::activate(std::vector<double> &v) const noexcept
{
    auto maxV = *std::max_element(v.begin(), v.end());
    v -= maxV;
    auto sum = std::accumulate(v.begin(), v.end(), 0., [](double init, double v) noexcept -> double {
        return init + std::exp(v);
    });
    apply(v, [&sum](double v) noexcept (true) -> double { return std::exp(v) / sum; });
}

std::vector<double> Softmax::derivative(std::vector<double> const &hypothesis) const noexcept
{
    auto smax = hypothesis;
    return smax * (1. - smax);
}

double Softmax::error(const TrainerValueHelper &values) const noexcept
{
    auto combined = boost::combine(values.hypothesis(), values.expectation());
    double crossEntropy = std::accumulate(combined.begin(), combined.end(), 0., [](double init, auto tuple) -> double {
        double hypothesis, expectation;
        boost::tie(hypothesis, expectation) = tuple;
        return init + (expectation * std::log(hypothesis));
    });
    return -crossEntropy;
}

std::vector<double> Softmax::lossDerivative(const std::vector<double> &hypothesis, const std::vector<double> &expectations) const noexcept
{
    std::vector<double> dLoss = expectations - hypothesis;
    return -dLoss;
}

} // namespace activation
} // namespace models
} // namespace mimir
