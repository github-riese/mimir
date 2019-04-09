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
    void activate(std::vector<double> &) const noexcept override;
    std::vector<double> derivative(std::vector<double> const &hypothesis) const noexcept override;
    double error(mimir::models::TrainerValueHelper const &) const noexcept override;
    std::vector<double> lossDerivative(const std::vector<double> &hypothesis, const std::vector<double> &expectations) const noexcept override;
};

} // namespace activation
} // namespace models
} // namespace mimir

#endif // SOFTMAX_H
