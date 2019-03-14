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

void Sigmoid::derivative(std::vector<double> &v) const noexcept
{
    activate(v);
    std::transform(v.begin(), v.end(), v.begin(), [](double sigmoid){
        return sigmoid * (1-sigmoid);
    });
}

std::vector<double> Sigmoid::loss(const std::vector<double> &hypothesis, const std::vector<double> &expectation) const noexcept
{
    return expectation - hypothesis;
}


} // namespace activation
} // namespace models
} // namespace mimir
