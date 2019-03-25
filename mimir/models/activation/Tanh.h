#ifndef TANH_H
#define TANH_H

#include <mimir/models/activation/AbstractMSELossActivator.h>

namespace mimir {
namespace models {
namespace activation {

class Tanh : public AbstractMSELossActivator
{
public:

    // ActivationInterface interface
public:
    std::string name() const override;
    void activate(std::vector<double> &) const noexcept override;
    void derivative(std::vector<double> &) const noexcept override;
};

} // namespace activation
} // namespace models
} // namespace mimir

#endif // TANH_H
