#ifndef ACTIVATIONINTERFACE_H
#define ACTIVATIONINTERFACE_H

#include <string>
#include <vector>

namespace mimir {
namespace models {
namespace activation {

class ActivationInterface
{
public:
    virtual ~ActivationInterface() = default;
    virtual std::string name() const = 0;
    virtual void activate(std::vector<double> &) const noexcept = 0;
    virtual void derivative(std::vector<double> &) const noexcept = 0;
};

} // namespace activation
} // namespace helpers
} // namespace mimir

#endif // ACTIVATIONINTERFACE_H
