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
    void derivative(std::vector<double> &) const noexcept override;

};

} // namespace activation
} // namespace models
} // namespace mimir

#endif // SIGMOID_H
