#ifndef MAXCLASSTURNOUTDETECTOR_H
#define MAXCLASSTURNOUTDETECTOR_H

#include "services/detect/AbstractDependencyDetector.h"

namespace mimir {
namespace services {
namespace detect {

class MaxClassTurnOutDetector : public AbstractDependencyDetector
{
public:
    MaxClassTurnOutDetector(models::CPT &cpt, long classFielIndex, models::ColumnIndexValuePairVector const &sample);
    std::vector<models::ColumnIndexValuePairVector> calculateDirectClassParents(models::ProbabilityDistribution const &, size_t maxResults) override;
};

} // namespace detect
} // namespace services
} // namespace mimir

#endif // MAXCLASSTURNOUTDETECTOR_H
