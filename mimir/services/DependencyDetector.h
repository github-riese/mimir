#ifndef DEPENDENCYDETECTOR_H
#define DEPENDENCYDETECTOR_H

#include <utility>
#include <vector>

#include "../models/CPT.h"
#include "../models/ValueIndex.h"
#include "../models/KeyValuePair.h"
#include "../models/BayesNetFragment.h"

namespace mimir {
namespace services {

class DependencyDetector
{
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
    models::BayesNetFragment findPredictionGraph(const models::ValueIndex nameToPredict, const models::ColumnNameValuePairVector &input, NameResolver &nr);
private:
    models::BayesNetFragmentVector findAnyGraph(const models::ValueIndex nameToPredict, models::ColumnNameValuePairVector const &input, NameResolver &nr);
    models::Probability likelihood(models::ColumnIndexValuePair const &k, models::ColumnIndexValuePairVector const &input) const;
    models::Probability conditionalProbability(models::ColumnIndexValuePair const&, models::ColumnIndexValuePairVector const &);
    void eliminateZeroEvidence(std::vector<models::ColumnIndexValuePair> &) const;
    models::ColumnNameValuePairVector indexedPairVectorToNamedPairVector(models::ColumnIndexValuePairVector const &) const;
    models::ColumnIndexValuePairVector namedPairVectorToIndexedPairVector(models::ColumnNameValuePairVector const &) const;
private:
    models::CPT &_cpt;
};

} // namespace services
} // namespace mimir

#endif // DEPENDENCYDETECTOR_H
