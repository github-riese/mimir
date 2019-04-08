#ifndef SOFTMAX_H
#define SOFTMAX_H

#include "ActivationInterface.h"

#include <mimir/models/Matrix.h>

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
    std::vector<double> activationPrime(std::vector<double> const &) const noexcept override;
    std::vector<double> activationPrime(std::vector<double> const &hypothesis, std::vector<double> const &input) const noexcept override;
    double error(mimir::models::TrainerValueHelper const &) const noexcept override;
private:
    Matrix jacobian(std::vector<double> const &hypothesis) const noexcept;
};

} // namespace activation
} // namespace models
} // namespace mimir

#endif // SOFTMAX_H
