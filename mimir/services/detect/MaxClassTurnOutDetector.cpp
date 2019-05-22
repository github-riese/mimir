#include "MaxClassTurnOutDetector.h"

#include "helpers/helpers.h"

using std::vector;
using std::pair;

using mimir::models::ColumnIndexValuePairVector;
using mimir::models::detect::InternalFragment;
using mimir::models::detect::InternalFragmentVector;

using mimir::helpers::containerHas;

namespace mimir {
namespace services {
namespace detect {

using VectorLengthOfFields = pair<ColumnIndexValuePairVector, double>;

struct DescSorter {
    bool operator()(VectorLengthOfFields const &left, VectorLengthOfFields const &right) const
    {
        return right.second < left.second;
    }
};

MaxClassTurnOutDetector::MaxClassTurnOutDetector(models::CPT &cpt, long classFielIndex, models::ColumnIndexValuePairVector const &sample) :
    AbstractDependencyDetector(cpt, classFielIndex, sample)
{

}

std::vector<models::ColumnIndexValuePairVector> MaxClassTurnOutDetector::calculateDirectClassParents(const models::ProbabilityDistribution &baseProbability, size_t maxResults)
{
    if (maxResults == 0)
        return {};
    vector<VectorLengthOfFields> vectorLengthByField;
    auto parameters = _sampleFields;
    sort(parameters, false);
    for (auto field : parameters) {
        auto classification = _cpt.classify(_classFieldIndex, {field});
        if (classification.vectorLength() > baseProbability.vectorLength()) {
            vectorLengthByField.push_back({{field}, classification.vectorLength()});
        }
        for (auto previous : vectorLengthByField) {
            auto v = previous.first;
            if (containerHas(v, field)) {
                continue;
            }
            v.push_back(field);
            auto t = _cpt.classify(_classFieldIndex, v);
            if (t.vectorLength() > previous.second) {
                sort(v.begin(), v.end());
                vectorLengthByField.push_back({v, t.vectorLength()});
            }
        }
    }
    sort(vectorLengthByField.begin(), vectorLengthByField.end(), DescSorter());
    auto pick = vectorLengthByField.begin();
    std::vector<models::ColumnIndexValuePairVector> baseParents;
    for (auto n = 0u; n < maxResults && pick != vectorLengthByField.end(); ++n, ++pick) {
        baseParents.push_back(pick->first);
    }
    return baseParents;
}

} // namespace detect
} // namespace services
} // namespace mimir
