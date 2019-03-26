#ifndef ACTIVATIONINTERFACE_H
#define ACTIVATIONINTERFACE_H

#include <string>
#include <vector>
#include <models/Matrix.h>

namespace mimir {
namespace models {
namespace activation {

class ActivationInterface
{
public:
    virtual ~ActivationInterface() = default;
    virtual std::string name() const = 0;
    virtual void activate(std::vector<double> &) const noexcept = 0;
    virtual std::vector<double> delta(std::vector<double> const &zValues, std::vector<double> const &costDerivative) const noexcept = 0;
    virtual std::vector<double> biasGradient(std::vector<double> const &biases, std::vector<double> const &costDerivative) const noexcept = 0;
    virtual Matrix weightGradient(Matrix const &weights, std::vector<double> const &costDerivative) const noexcept = 0;
    virtual double loss(std::vector<std::vector<double>> const &results, std::vector<std::vector<double>> const &expectations) const noexcept = 0;
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
