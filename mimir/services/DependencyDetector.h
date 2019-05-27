#ifndef DEPENDENCYDETECTOR_H
#define DEPENDENCYDETECTOR_H

#include <utility>
#include <vector>

#include "../models/CPT.h"
#include "../models/ValueIndex.h"
#include "../models/KeyValuePair.h"
#include "../models/BayesNetFragment.h"
#include "../models/detect/DetectionStrategy.h"

namespace mimir {
namespace services {

class DependencyDetector
{
    using PriorMap = std::map<long, models::Node>;

public:
    DependencyDetector(models::CPT &cpt);
    models::NodeVector computePriors(const mimir::models::ColumnNameValuePairVector &input) const;
    mimir::models::NodeVector computePriors(const models::ColumnIndexValuePairVector &input) const;
    /**
     * @brief find a non cyclic directed graph in input
     * Returns the graph resulting in the longest vector
     * @param index to fieldName(!) whose value shall be predicted from input and CPT
     * @param input configuration to dare a prediction for classification
     * @param nr
     * @return
     */
    models::BayesNet findPredictionGraph(const models::ValueIndex nameToPredict, const models::ColumnNameValuePairVector &input, size_t maxGrapths, models::detect::DetectStrategy strategy = models::detect::MaximizeOutcome);
private:
    void buildPriorMap();
    models::BayesNetFragmentVector findAnyGraph(size_t maxToEvaluate);

    void buildGraph(models::ColumnIndexValuePairVector const &) const;

    models::Probability likelihood(models::ColumnIndexValuePair const &k, models::ColumnIndexValuePairVector const &input) const;
    models::Probability conditionalProbability(models::ColumnIndexValuePair const&, models::ColumnIndexValuePairVector const &);
    void eliminateZeroEvidence(models::ColumnIndexValuePairVector &) const;
    models::ColumnNameValuePairVector indexedPairVectorToNamedPairVector(models::ColumnIndexValuePairVector const &) const;
    models::ColumnIndexValuePairVector namedPairVectorToIndexedPairVector(models::ColumnNameValuePairVector const &) const;

    models::Probability priorOf(const models::ColumnIndexValuePair &value) const;
private:
    models::CPT &_cpt;
    models::ValueIndex _className;
    long _classIndex = -1;
    models::ColumnIndexValuePairVector _examinedParams;
    PriorMap _priors;
};

} // namespace services
} // namespace mimir

#endif // DEPENDENCYDETECTOR_H
