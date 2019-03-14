#ifndef TANH_H
#define TANH_H

#include <mimir/models/activation/ActivationInterface.h>

namespace mimir {
namespace models {
namespace activation {

class Tanh : public ActivationInterface
{
public:

    // ActivationInterface interface
public:
    std::string name() const override;
    void activate(std::vector<double> &) const noexcept override;
    void derivative(std::vector<double> &) const noexcept override;
    std::vector<double> loss(const std::vector<double> &, const std::vector<double> &) const noexcept override;
};

} // namespace activation
} // namespace models
} // namespace mimir

#endif // TANH_H
