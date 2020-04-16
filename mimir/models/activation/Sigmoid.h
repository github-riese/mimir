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
    void activate(std::vector<float> &) const noexcept override;
    std::vector<float> derivative(std::vector<float> const &hypothesis) const noexcept override;
protected:
};

} // namespace activation
} // namespace models
} // namespace mimir

#endif // SIGMOID_H
