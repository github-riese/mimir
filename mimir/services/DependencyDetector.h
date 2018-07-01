#ifndef DEPENDENCYDETECTOR_H
#define DEPENDENCYDETECTOR_H

#include <utility>
#include <vector>

#include "../models/CPT.h"
#include "../models/ValueIndex.h"
#include "../models/KeyValuePair.h"

namespace mimir {
namespace services {

class DependencyDetector
{
public:
    DependencyDetector(models::CPT &cpt);
    std::vector<std::vector<models::ValueIndex>> detectDependencies(const std::vector<models::ValueIndex> &values, models::ValueIndex classifiingColumn, NameResolver &ns);

private:
    models::Probability conditionalProbability(models::ColumnIndexValuePair const&, std::vector<models::ColumnIndexValuePair>const &);
private:
    models::CPT &_cpt;
};

} // namespace services
} // namespace mimir

#endif // DEPENDENCYDETECTOR_H
