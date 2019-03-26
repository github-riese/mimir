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

std::vector<double> Sigmoid::delta(const std::vector<double> &zValues, const std::vector<double> &costDerivative) const noexcept
{
    auto result = zValues;
    activate(result);
    std::transform(result.begin(), result.end(), result.begin(), [](double sigmoid){
        return sigmoid * (1-sigmoid);
    });
    return result *= costDerivative;
}

std::vector<double> Sigmoid::biasGradient(std::vector<double> const &biases, std::vector<double> const &dCost) const noexcept
{
}

Matrix Sigmoid::weightGradient(const Matrix &weights, const std::vector<double> &costDerivative) const noexcept
{

}

} // namespace activation
} // namespace models
} // namespace mimir
