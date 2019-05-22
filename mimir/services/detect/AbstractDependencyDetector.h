#ifndef ABSTRACTDEPENDENCYDETECTOR_H
#define ABSTRACTDEPENDENCYDETECTOR_H

#include "models/CPT.h"
#include "models/detect/InternalNodeTree.h"
#include "models/ProbabilityDistribution.h"

namespace mimir {
namespace services {
namespace detect {

class AbstractDependencyDetector
{
public:
    AbstractDependencyDetector(models::CPT &cpt, long classFielIndex, models::ColumnIndexValuePairVector const &sample);
    virtual ~AbstractDependencyDetector() = default;
    virtual std::vector<models::ColumnIndexValuePairVector> calculateDirectClassParents(models::ProbabilityDistribution const &, size_t maxResults) = 0;

protected:
    models::CPT &_cpt;
    long _classFieldIndex;
    models::ColumnIndexValuePairVector const &_sampleFields;
};

} // namespace detect
} // namespace services
} // namespace mimir

#endif // ABSTRACTDEPENDENCYDETECTOR_H
