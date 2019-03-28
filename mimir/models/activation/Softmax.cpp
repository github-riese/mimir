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
    auto sum = std::accumulate(v.begin(), v.end(), 0., [](double init, double v) noexcept -> double {
        return init + std::exp(v);
    });
    apply(v, [&sum](double v) noexcept (true) -> double { return std::exp(v) / sum; });
}

std::vector<double> Softmax::biasGradient(const std::vector<double> &, const std::vector<double> &costDerivative) const noexcept
{
    return costDerivative;
}

double Softmax::loss(const std::vector<std::vector<double> > &hypotheses, const std::vector<std::vector<double> > &expectations) const noexcept
{
    double crossEntropy = .0;
    auto h = hypotheses.begin();
    auto e = expectations.begin();
    for (; h != hypotheses.end(); ++h, ++e) {
        auto combined = boost::combine(*h, *e);
        crossEntropy += std::accumulate(combined.begin(), combined.end(), 0., [](double init, auto tuple) -> double {
            double hypothesis, expectation;
            boost::tie(hypothesis, expectation) = tuple;
            return init + (expectation * std::log(hypothesis));
        }) / -static_cast<double>(combined.size());
    }
    return crossEntropy / static_cast<double>(hypotheses.size());
}

} // namespace activation
} // namespace models
} // namespace mimir
