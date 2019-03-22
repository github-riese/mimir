#ifndef SIGMOID_H
#define SIGMOID_H

#include <mimir/models/activation/ActivationInterface.h>

namespace mimir {
namespace models {
namespace activation {

class Sigmoid : public ActivationInterface
{
public:
    std::string name() const override;
    void activate(std::vector<double> &) const noexcept override;
    void derivative(std::vector<double> &) const noexcept override;

};

} // namespace activation
} // namespace models
} // namespace mimir

#endif // SIGMOID_H
