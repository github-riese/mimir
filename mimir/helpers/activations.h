#ifndef ACTIVATIONS_H
#define ACTIVATIONS_H

namespace mimir {
namespace helpers {

struct Activation
{
    virtual ~Activation() = default;
    virtual double activate(double) const = 0;
    virtual double derivative(double) const = 0;
};

struct Sigmoid : Activation
{
    double activate(double) const override;
    double derivative(double) const override;
};

struct RectifiedLinear : Activation
{
    double activate(double) const override;
    double derivative(double) const override;
};

} // namespace helpers
} // namespace mimir

#endif // ACTIVATIONS_H
