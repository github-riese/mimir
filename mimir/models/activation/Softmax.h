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
    std::vector<double> derivative(std::vector<double> const &) const noexcept override;
    double loss(mimir::models::TrainerValueHelper const &) const noexcept override;
};

} // namespace activation
} // namespace models
} // namespace mimir

#endif // SOFTMAX_H
