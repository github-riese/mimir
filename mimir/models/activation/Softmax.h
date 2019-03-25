#ifndef SOFTMAX_H
#define SOFTMAX_H

#include "AbstractMSELossActivator.h"

namespace mimir {
namespace models {
namespace activation {

class Softmax : public AbstractMSELossActivator
{
public:

    // ActivationInterface interface
public:
    std::string name() const override;
    void activate(std::vector<double> &) const noexcept override;
    void derivative(std::vector<double> &) const noexcept override;
    double loss(std::vector<std::vector<double>> const &hypotheses, std::vector<std::vector<double>> const &expectations) const noexcept override;
};

} // namespace activation
} // namespace models
} // namespace mimir

#endif // SOFTMAX_H
