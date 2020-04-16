#ifndef SOFTMAX_H
#define SOFTMAX_H

#include "ActivationInterface.h"

namespace mimir {
namespace models {
namespace activation {

class Softmax : public ActivationInterface
{
public:

    // ActivationInterface interface
public:
    std::string name() const override;
    void activate(std::vector<float> &) const noexcept override;
    std::vector<float> derivative(std::vector<float> const &hypothesis) const noexcept override;
    float error(mimir::models::TrainerValueHelper const &) const noexcept override;
    std::vector<float> lossDerivative(const std::vector<float> &hypothesis, const std::vector<float> &expectations) const noexcept override;
};

} // namespace activation
} // namespace models
} // namespace mimir

#endif // SOFTMAX_H
