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
    using VecorLengthOfField = std::pair<models::ColumnIndexValuePairVector, models::Probability>;
    using VectorLengthOfFieldVector = std::vector<VecorLengthOfField>;
    struct FieldLikelihood { models::ColumnIndexValuePair field; models::ColumnIndexValuePairVector parents; models::Probability probability;};
    using FieldLikelihoodVector = std::vector<FieldLikelihood>;

public:
    enum class Strategy
    {
        /**
         * in case of multiple trees chose the first at hand.
         */
        DontCare,
        /**
         * in case of multiple trees choose the one with broadest nodes. I.e. the one with the most relatively independent values.
         */
        PreferWidth,
        /**
         * in case of multiple trees choose the one with greatest depth.
         */
        PreferDepth
    };
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
    models::BayesNetFragment findPredictionGraph(const models::ValueIndex nameToPredict, const models::ColumnNameValuePairVector &input, size_t maxGrapths, Strategy strategy = Strategy::DontCare);
private:
    models::BayesNetFragmentVector findAnyGraph(const models::ValueIndex nameToPredict, models::ColumnNameValuePairVector const &input, size_t maxToEvaluate) const;

    VectorLengthOfFieldVector maxAPosteoriLevel0(const models::ValueIndex nameToPredict, const models::ColumnIndexValuePairVector &indexedFields, size_t maxToEvaluate) const;
    FieldLikelihoodVector maximizeLikelyhoods(const models::ColumnIndexValuePairVector &fields) const;

    void buildGraph(VecorLengthOfField const &, FieldLikelihoodVector const &) const;

    models::Probability likelihood(models::ColumnIndexValuePair const &k, models::ColumnIndexValuePairVector const &input) const;
    models::Probability conditionalProbability(models::ColumnIndexValuePair const&, models::ColumnIndexValuePairVector const &);
    void eliminateZeroEvidence(models::ColumnIndexValuePairVector &) const;
    models::ColumnNameValuePairVector indexedPairVectorToNamedPairVector(models::ColumnIndexValuePairVector const &) const;
    models::ColumnIndexValuePairVector namedPairVectorToIndexedPairVector(models::ColumnNameValuePairVector const &) const;
private:
    models::CPT &_cpt;
};

} // namespace services
} // namespace mimir

#endif // DEPENDENCYDETECTOR_H
