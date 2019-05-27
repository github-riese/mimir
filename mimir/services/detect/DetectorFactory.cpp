#include "DetectorFactory.h"
#include "MaxClassTurnOutDetector.h"

namespace mimir {
namespace services {
namespace detect {

DetectorFactory::DetectorFactory(models::CPT &cpt, long classFieldIndex, const models::ColumnIndexValuePairVector &sample) noexcept :
    _cpt(cpt),
    _classFieldIndex(classFieldIndex),
    _sample(sample)
{
}

SharedDetector DetectorFactory::getDetector(models::detect::DetectStrategy strategy)
{
    switch (strategy) {
        case models::detect::DetectStrategy::MaximizeOutcome:
        return std::make_shared<MaxClassTurnOutDetector>(_cpt, _classFieldIndex, _sample);
    default:
        return std::shared_ptr<DependencyDetectionInterface>();
    }
}


} // namespace detect
} // namespace services
} // namespace mimir
