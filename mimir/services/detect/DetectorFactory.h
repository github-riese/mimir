#ifndef DETECTORFACTORY_H
#define DETECTORFACTORY_H

#include <memory>

#include <models/detect/DetectionStrategy.h>
#include <models/CPT.h>
#include <models/KeyValuePair.h>

namespace mimir {
namespace services {
namespace detect {

class DependencyDetectionInterface;
using SharedDetector = std::shared_ptr<DependencyDetectionInterface>;

class DetectorFactory
{
public:
    DetectorFactory(models::CPT &cpt, long classFieldIndex, models::ColumnIndexValuePairVector const &sample) noexcept;
    SharedDetector getDetector(models::detect::DetectStrategy);
private:
    models::CPT &_cpt;
    long _classFieldIndex;
    models::ColumnIndexValuePairVector const &_sample;
};
} // namespace detect
} // namespace services
} // namespace mimir

#endif // DETECTORFACTORY_H
