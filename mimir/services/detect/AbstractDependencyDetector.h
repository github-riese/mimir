#ifndef ABSTRACTDEPENDENCYDETECTOR_H
#define ABSTRACTDEPENDENCYDETECTOR_H

#include "models/CPT.h"
#include "models/ProbabilityDistribution.h"
#include "DependencyDetectionInterface.h"

namespace mimir {
namespace services {
namespace detect {

class AbstractDependencyDetector : public DependencyDetectionInterface
{
public:
    AbstractDependencyDetector(models::CPT &cpt, long classFieldIndex, models::ColumnIndexValuePairVector const &sample);
    virtual ~AbstractDependencyDetector() = default;
    virtual models::detect::InternalNetVector buildNets(models::ProbabilityDistribution const &, size_t maxResults) = 0;
protected:
    models::CPT &_cpt;
    long _classFieldIndex;
    models::ColumnIndexValuePairVector const &_sampleFields;
};

} // namespace detect
} // namespace services
} // namespace mimir

#endif // ABSTRACTDEPENDENCYDETECTOR_H
