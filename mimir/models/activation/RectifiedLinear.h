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
    std::vector<double> derivative(std::vector<double> const &) const noexcept override;
};

} // namespace activation
} // namespace models
} // namespace mimir

#endif // RECTIFIEDLINEAR_H
