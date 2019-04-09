#include "Softmax.h"
#include <numeric>
#include <cmath>

#include <boost/range/combine.hpp>

#include <helpers/helpers.h>
#include <helpers/math.h>
#include <models/Layer.h>

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

std::vector<double> Softmax::derivative(std::vector<double> const &zValue) const noexcept
{
    std::vector<double> result(zValue.size());
    std::vector<double> ePowX(zValue.size());

    std::transform(zValue.begin(), zValue.end(), ePowX.begin(), [](double x) -> double { return std::exp(x); });
    std::vector<double> subEs(ePowX.size());
    for(auto i = 0u; i < ePowX.size(); ++i) {
        auto j = 0u;
        for (auto pow : ePowX) {
            if (i != j++) {
                subEs[i] += pow;
            }
        }
    }

    double divisor = std::accumulate(zValue.begin(), zValue.end(), 0., [](double init, double v) -> double {
        return init + std::exp(v);
    });
    divisor *= divisor;
    auto power = ePowX.begin();
    auto sum = subEs.begin();
    auto derivative = result.begin();
    for(; power != ePowX.end(); ++power, ++sum, ++derivative) {
        *derivative = *power * *sum/divisor;
    }

    return result;
/*
    auto const &hypothesis = layerData.hypothesis();
    auto h = hypothesis.begin();
    auto x = layerData.input().begin();
    auto y = layerData.expectation().begin();
    auto r = result.begin();
    for (; h != hypothesis.end(); ++h, ++x, ++r, ++y) {
        *r = *x * (*y - *h);
    }
    return -result;*/
}

double Softmax::loss(const TrainerValueHelper &values) const noexcept
{
    double crossEntropy = .0;
    auto combined = boost::combine(values.hypothesis(), values.expectation());
    crossEntropy += std::accumulate(combined.begin(), combined.end(), 0., [](double init, auto tuple) -> double {
        double hypothesis, expectation;
        boost::tie(hypothesis, expectation) = tuple;
        return init + (expectation * std::log(hypothesis));
    }) / -static_cast<double>(combined.size());
    return crossEntropy;
}

} // namespace activation
} // namespace models
} // namespace mimir
