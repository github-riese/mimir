#include "Softmax.h"
#include <numeric>
#include <cmath>

#include <boost/range/combine.hpp>

#include <helpers/helpers.h>
#include <helpers/math.h>
#include <models/AbstractLayer.h>

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

std::vector<double> Softmax::activationPrime(std::vector<double> const &hypothesis) const noexcept
{
    std::vector<double> t(hypothesis.size(), 1.);
    return activationPrime(hypothesis, t);
}

std::vector<double> Softmax::activationPrime(const std::vector<double> &hypothesis, const std::vector<double> &input) const noexcept
{
    return  std::vector<double>(hypothesis.size(), 1.);
    return (jacobian(hypothesis) * input).column(0);
}

double Softmax::error(const TrainerValueHelper &values) const noexcept
{
    double crossEntropy = .0;
    auto combined = boost::combine(values.hypothesis(), values.expectation());
    crossEntropy += std::accumulate(combined.begin(), combined.end(), 0., [](double init, auto tuple) -> double {
        double hypothesis, expectation;
        boost::tie(hypothesis, expectation) = tuple;
        return init + (expectation * std::log(hypothesis));
    }) / -static_cast<double>(combined.size());
    return crossEntropy/values.size();
}

Matrix Softmax::jacobian(const std::vector<double> &hypothesis) const noexcept
{
    size_t numNodes = hypothesis.size();
    mimir::models::Matrix jacobian(numNodes, numNodes);
    for (auto i = 0u; i < numNodes; ++i) {
        for (auto j = 0u; j < numNodes; ++j) {
            jacobian.setValue(i, j, hypothesis[i] * ((i==j ? 1. : 0. ) - hypothesis[j]));
        }
    }
    return jacobian;
}

} // namespace activation
} // namespace models
} // namespace mimir
