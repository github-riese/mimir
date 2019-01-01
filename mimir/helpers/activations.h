#ifndef ACTIVATIONS_H
#define ACTIVATIONS_H

#include <string>

namespace mimir {
namespace helpers {

struct Activation
{
    virtual ~Activation() = default;
    virtual std::string name() const = 0;
    virtual double activate(double) const = 0;
    virtual double derivative(double) const = 0;
};

struct Sigmoid : Activation
{
    std::string name() const override;
    double activate(double) const override;
    double derivative(double) const override;
};

struct RectifiedLinear : Activation
{
    std::string name() const override;
    double activate(double) const override;
    double derivative(double) const override;
};

} // namespace helpers
} // namespace mimir

#endif // ACTIVATIONS_H
