#ifndef ACTIVATIONINTERFACE_H
#define ACTIVATIONINTERFACE_H

#include <string>
#include <vector>

#include "models/BatchItem.h"
#include "models/TrainerValueHelper.h"

namespace mimir {
namespace models {
namespace activation {

class ActivationInterface
{
public:
    virtual ~ActivationInterface() = default;
    virtual std::string name() const = 0;
    virtual void activate(std::vector<double> &) const noexcept = 0;
    virtual std::vector<double> derivative(std::vector<double> const &hypothesis) const noexcept = 0;
    virtual std::vector<double> lossDerivative(std::vector<double> const &hypothesis, std::vector<double> const &expectations) const noexcept = 0;
    virtual double error(mimir::models::TrainerValueHelper const &values) const noexcept = 0;
protected:
    template<typename Op>
    inline void apply(std::vector<double> &vec, Op operation) const noexcept {
        std::transform(vec.begin(), vec.end(), vec.begin(), operation);
    }
};

} // namespace activation
} // namespace helpers
} // namespace mimir

#endif // ACTIVATIONINTERFACE_H
