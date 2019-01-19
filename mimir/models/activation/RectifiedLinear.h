#ifndef RECTIFIEDLINEAR_H
#define RECTIFIEDLINEAR_H

#include <mimir/models/activation/ActivationInterface.h>

namespace mimir {
namespace models {
namespace activation {

class RectifiedLinear : public ActivationInterface
{
public:

    // ActivationInterface interface
public:
    std::string name() const override;
    void activate(std::vector<double> &) const noexcept override ;
    void derivative(std::vector<double> &) const noexcept override ;
};

} // namespace activation
} // namespace models
} // namespace mimir

#endif // RECTIFIEDLINEAR_H