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
    void activate(std::vector<float> &) const noexcept override;
    std::vector<float> derivative(std::vector<float> const &) const noexcept override;
};

} // namespace activation
} // namespace models
} // namespace mimir

#endif // RECTIFIEDLINEAR_H
