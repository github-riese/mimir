#include "SigmoidLayer.h"

#include <mimir/helpers/math.h>

namespace mimir {
namespace models {
namespace layers {

SigmoidLayer::SigmoidLayer()
{

}

std::string SigmoidLayer::name() const noexcept
{
    return "sigmoid";
}

void SigmoidLayer::activate() noexcept
{
    std::transform(_hypothesis.begin(), _hypothesis.end(), _hypothesis.begin(), [](double v) -> double {
        return 1./(1.+std::exp(-v));
    });
}

std::vector<double> SigmoidLayer::gradientOnLoss(const std::vector<double> &expectation, const AbstractLayer * /* previousLayer */) noexcept
{
    auto loss = _hypothesis - expectation;
    return ::operator*(loss, derivative());
}

std::vector<double> SigmoidLayer::gradient(const AbstractLayer */* previousLayer */) noexcept
{
    return derivative();
}

std::vector<double> SigmoidLayer::derivative() const noexcept
{
    auto derivative = _hypothesis;
    std::transform(derivative.begin(), derivative.end(), derivative.begin(),
                   [](double sigmoid) noexcept -> double { return sigmoid * (1 - sigmoid); }
    );
    return derivative;
}

} // namespace layers
} // namespace models
} // namespace mimir
