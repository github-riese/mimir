#include "AbstractDependencyDetector.h"

namespace mimir {
namespace services {
namespace detect {

using models::ColumnNameValuePairVector;
using models::ColumnIndexValuePairVector;

AbstractDependencyDetector::AbstractDependencyDetector(models::CPT &cpt, long classFieldIndex, models::ColumnIndexValuePairVector const &sample) :
    _cpt(cpt),
    _classFieldIndex(classFieldIndex),
    _sampleFields(sample)
{
}


} // namespace detect
} // namespace services
} // namespace mimir
