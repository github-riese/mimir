#include "RectifiedLinearUnitLayer.h"

#include <mimir/helpers/math.h>

namespace mimir {
namespace models {
namespace layers {

RectifiedLinearUnitLayer::RectifiedLinearUnitLayer()
{

}

std::string RectifiedLinearUnitLayer::name() const noexcept
{
    return "relu";
}

void RectifiedLinearUnitLayer::activate() noexcept
{
    std::transform(_hypothesis.begin(), _hypothesis.end(), _hypothesis.begin(),
                   [] (double v) noexcept -> double {
        return v > 0. ? v : 0;
    });
}

std::vector<double> RectifiedLinearUnitLayer::gradientOnLoss(const std::vector<double> &expectation, const AbstractLayer * /* previousLayer */) noexcept
{
    return (_hypothesis - expectation) * derivative();
}

std::vector<double> RectifiedLinearUnitLayer::gradient(const AbstractLayer * /* previous */) noexcept
{
    return derivative();
}

std::vector<double> RectifiedLinearUnitLayer::derivative() const noexcept
{
    auto hypPrime = _hypothesis;
    std::transform(hypPrime.begin(), hypPrime.end(), hypPrime.begin(), [](double v) noexcept -> double {
        return v > 0 ? 1 : 0;
    });
    return hypPrime;
}

} // namespace layers
} // namespace models
} // namespace mimir
