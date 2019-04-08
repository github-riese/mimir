#include "Softmax.h"

#include <mimir/helpers/math.h>

using mimir::models::Matrix;

namespace mimir {
namespace models {
namespace layers {

Softmax::Softmax()
{

}

std::string Softmax::name() const noexcept
{
    return "softmax";
}

std::vector<double> Softmax::gradientOnLoss(const std::vector<double> &expectation, const AbstractLayer *previousLayer) noexcept
{
    size_t numNodes = _hypothesis.size();
    mimir::models::Matrix jacobian(numNodes, numNodes);
    for (auto i = 0u; i < numNodes; ++i) {
        for (auto j = 0u; j < numNodes; ++j) {
            jacobian.setValue(i, j, _hypothesis[i] * ((i==j ? 1. : 0. ) - _hypothesis[j]));
        }
    }
    return (Matrixjacobian).column(0);
}

std::vector<double> Softmax::gradient(const AbstractLayer *previous) noexcept
{

}

void Softmax::activate() noexcept
{
    auto maxV = *std::max_element(_hypothesis.begin(), _hypothesis.end());
    _hypothesis -= maxV;
    auto sum = std::accumulate(_hypothesis.begin(), _hypothesis.end(), 0., [](double init, double v) noexcept -> double {
        return init + std::exp(v);
    });
    std::transform(_hypothesis.begin(), _hypothesis.end(), _hypothesis.begin(), [sum](double v) noexcept (true) -> double { return std::exp(v) / sum; });

}

} // namespace layers
} // namespace models
} // namespace mimir
