#include "Softmax.h"
#include <numeric>
#include <cmath>
#include <boost/range/combine.hpp>

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

std::vector<double> Softmax::loss(const std::vector<double> &hypothesis, const std::vector<double> &expectation) const noexcept
{
    auto combined = boost::combine(hypothesis, expectation);
    double crossEntropy = std::accumulate(combined.begin(), combined.end(), 0., [](double init, auto tuple) -> double {
        double hypothesis, expectation;
        boost::tie(hypothesis, expectation) = tuple;
        return init + (expectation * std::log(hypothesis) +  (1 - expectation) * std::log(1 - hypothesis));
    }) / -static_cast<double>(hypothesis.size());

}

} // namespace activation
} // namespace models
} // namespace mimir
