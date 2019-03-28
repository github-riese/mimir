#ifndef RECTIFIEDLINEAR_H
#define RECTIFIEDLINEAR_H

#include <mimir/models/activation/AbstractMSELossActivator.h>

namespace mimir {
namespace models {
namespace activation {

class RectifiedLinear : public AbstractMSELossActivator
{
public:

    // ActivationInterface interface
public:
    std::string name() const override;
    void activate(std::vector<double> &) const noexcept override;
    std::vector<double> biasGradient(std::vector<double> const &hypothesis, std::vector<double> const &costDerivative) const noexcept override;
private:
    void derivative(std::vector<double> &) const noexcept;
};

} // namespace activation
} // namespace models
} // namespace mimir

#endif // RECTIFIEDLINEAR_H
