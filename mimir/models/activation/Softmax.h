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
    std::vector<double> biasGradient(std::vector<double> const &hypothesis, std::vector<double> const &costDerivative) const noexcept override;
    double loss(std::vector<std::vector<double>> const &hypotheses, std::vector<std::vector<double>> const &expectations) const noexcept override;
};

} // namespace activation
} // namespace models
} // namespace mimir

#endif // SOFTMAX_H
