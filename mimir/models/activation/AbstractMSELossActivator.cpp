#include "AbstractMSELossActivator.h"
#include <mimir/helpers/math.h>

namespace mimir {
namespace models {
namespace activation {

double AbstractMSELossActivator::loss(const std::vector<std::vector<double> > &results, const std::vector<std::vector<double> > &expectations) const noexcept
{
    double loss = .0;
    auto result = results.begin();
    auto expectation = expectations.begin();
    for (;result != results.end(); ++result, ++expectation) {
        auto diff = *result - *expectation;
        diff *= diff;
        loss += std::accumulate(diff.begin(), diff.end(), 0.)/static_cast<double>(diff.size());
    }
    return loss/static_cast<double>(results.size());
}

} // namespace activation
} // namespace models
} // namespace mimir
