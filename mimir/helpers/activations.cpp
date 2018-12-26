#include "activations.h"

#include <algorithm>
#include <cmath>

namespace mimir {
namespace helpers {


double Sigmoid::activate(double z) const
{
    return 1./1.+(std::exp(-z));
}

double Sigmoid::derivative(double z) const
{
    auto sigmoid = activate(z);
    return sigmoid * (1-sigmoid);
}

double RectifiedLinear::activate(double z) const
{
    return std::max(0., z);
}

double RectifiedLinear::derivative(double z) const
{
    return z < 0. ? 0. : 1.;
}

} // namespace helpers
} // namespace mimir
