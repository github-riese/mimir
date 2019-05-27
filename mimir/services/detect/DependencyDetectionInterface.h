#ifndef DEPENDENCYDETECTIONINTERFACE_H
#define DEPENDENCYDETECTIONINTERFACE_H

#include <vector>
#include <models/KeyValuePair.h>
#include <models/CPT.h>
#include <models/detect/InternalNodeTree.h>
#include <models/NamedProbability.h>


namespace mimir {
namespace services {
namespace detect {
class DependencyDetectionInterface
{
public:
    virtual ~DependencyDetectionInterface() = default;
    virtual models::detect::InternalNetVector buildNets(models::ProbabilityDistribution const &, size_t maxResults) = 0;
};
} // namespace detect
} // namespace services
} // namespace mimir

#endif // DEPENDENCYDETECTIONINTERFACE_H
