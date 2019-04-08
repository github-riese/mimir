#include "AbstractMSELossActivator.h"

#include <mimir/models/Layer.h>
#include <mimir/helpers/math.h>

namespace mimir {
namespace models {
namespace activation {

double AbstractMSELossActivator::error(const mimir::models::TrainerValueHelper &item) const noexcept
{
    auto diff = item.hypothesis() - item.expectation();
    diff *= diff;
    return  std::accumulate(diff.begin(), diff.end(), 0.)/static_cast<double>(diff.size());
}

} // namespace activation
} // namespace models
} // namespace mimir
