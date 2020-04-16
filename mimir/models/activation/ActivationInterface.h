#ifndef ACTIVATIONINTERFACE_H
#define ACTIVATIONINTERFACE_H

#include <string>
#include <vector>

#include <mimir/models/neuronnet/BatchItem.h>
#include <mimir/models/neuronnet/TrainerValueHelper.h>

namespace mimir {
namespace models {
namespace activation {

class ActivationInterface
{
public:
    virtual ~ActivationInterface() = default;
    virtual std::string name() const = 0;
    virtual void activate(std::vector<float> &) const noexcept = 0;
    virtual std::vector<float> derivative(std::vector<float> const &hypothesis) const noexcept = 0;
    virtual std::vector<float> lossDerivative(std::vector<float> const &hypothesis, std::vector<float> const &expectations) const noexcept = 0;
    virtual float error(mimir::models::TrainerValueHelper const &values) const noexcept = 0;
protected:
    template<typename Op>
    inline void apply(std::vector<float> &vec, Op operation) const noexcept {
        std::transform(vec.begin(), vec.end(), vec.begin(), operation);
    }
};

} // namespace activation
} // namespace helpers
} // namespace mimir

#endif // ACTIVATIONINTERFACE_H
