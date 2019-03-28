#include "Sigmoid.h"

#include <cmath>

#include "../../helpers/math.h"

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

std::vector<double> Sigmoid::biasGradient(const std::vector<double> &hypothesis, const std::vector<double> &costDerivative) const noexcept
{
    auto hypothesisDerivative = hypothesis;
    derivative(hypothesisDerivative);
    return hypothesisDerivative *= costDerivative;
}

void Sigmoid::derivative(std::vector<double> &input) const noexcept
{
    std::transform(input.begin(), input.end(), input.begin(), [](double sigmoid){
        return sigmoid * (1-sigmoid);
    });
}

} // namespace activation
} // namespace models
} // namespace mimir
