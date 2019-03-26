#ifndef SIGMOID_H
#define SIGMOID_H

#include <mimir/models/activation/AbstractMSELossActivator.h>

namespace mimir {
namespace models {
namespace activation {

class Sigmoid : public AbstractMSELossActivator
{
public:
    std::string name() const override;
    void activate(std::vector<double> &) const noexcept override;
    std::vector<double> delta(const std::vector<double> &zValues, const std::vector<double> &costDerivative) const noexcept override;
    std::vector<double> biasGradient(const std::vector<double> &z, std::vector<double> const &dCost) const noexcept override;
    Matrix weightGradient(const Matrix &weights, const std::vector<double> &costDerivative) const noexcept override;
};

} // namespace activation
} // namespace models
} // namespace mimir

#endif // SIGMOID_H
