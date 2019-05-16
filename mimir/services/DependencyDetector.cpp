#include "DependencyDetector.h"

#include <algorithm>
#include <deque>
#include <map>
#include <vector>

#include <iostream>

#include <iotaomegapsi/tools/logger/Logger.h>

#include "../helpers/helpers.h"
#include "../models/BayesNetFragment.h"
#include "../models/Probability.h"
#include <iotaomegapsi/tools/timer/Timing.h>

using std::back_inserter;
using std::deque;
using std::pair;
using std::map;
using std::vector;

using std::copy;
using std::find_if;
using std::sort;

using std::cout;
using std::cerr;
using std::endl;

using mimir::helpers::containerHas;

using mimir::models::BayesNetFragment;
using mimir::models::BayesNetFragmentVector;
using mimir::models::CPT;
using mimir::models::ColumnIndexValuePair;
using mimir::models::ColumnIndexValuePairVector;
using mimir::models::ColumnNameValuePair;
using mimir::models::ColumnNameValuePairVector;
using mimir::models::Node;
using mimir::models::NodeVector;
using mimir::models::BayesNetFragment;
using mimir::models::Probability;
using mimir::models::ValueIndex;

using iotaomegapsi::tools::timer::LoggingTiming;

namespace mimir {
namespace services {

DependencyDetector::DependencyDetector(CPT &cpt) :
    _cpt(cpt)
{
}

NodeVector DependencyDetector::computePriors(const models::ColumnNameValuePairVector &input) const
{
    return computePriors(namedPairVectorToIndexedPairVector(input));
}

NodeVector DependencyDetector::computePriors(const models::ColumnIndexValuePairVector &input) const
{
    // a list. for each input - P(xn=in)
    // Probabilities delievered as vector of Nodes
    // sorted by probability ascending
    LoggingTiming<std::chrono::microseconds> _timer(__PRETTY_FUNCTION__);
    vector<models::Node> priorNodes;
    for (auto value : input) {
        Probability p = _cpt.probability({value});
        priorNodes.push_back({_cpt.fieldName(value.columnIndex), value.value, p});
    }
    sort(priorNodes.begin(), priorNodes.end());
    return priorNodes;
}

BayesNetFragment DependencyDetector::findPredictionGraph(const models::ValueIndex nameToPredict, const models::ColumnNameValuePairVector &input, size_t maxGrapths, Strategy strategy)
{
    findAnyGraph(nameToPredict, input, maxGrapths);
    return BayesNetFragment();
}

BayesNetFragmentVector DependencyDetector::findAnyGraph(const models::ValueIndex nameToPredict, const models::ColumnNameValuePairVector &input, size_t maxToEvaluate) const
{
    LoggingTiming<std::chrono::microseconds> _timer(__PRETTY_FUNCTION__);
    /* This is what we'll do:
     * 1. we get the probabilities of each parameter
     * 2. we get the likelihood of each parameter given any other parameter if the likelihood is greater than the probability of the parameter alone.
     * 3. we find the max a posteoris (whithout acutally classifiing!) given any combination of input. Of those we choose maxToEvaluate. This will be the root of the graph.
     * 4. for each result of 3 we now build a parameter tree that contains all the parameters.
     */
    auto indexedFields = namedPairVectorToIndexedPairVector(input);
    auto likelihoods = maximizeLikelyhoods(indexedFields);
    auto graphBases = maxAPosteoriLevel0(nameToPredict, indexedFields, maxToEvaluate);
    for (auto graphBase : graphBases)
    {
        buildGraph(graphBase, likelihoods);
    }

    return {};
}

DependencyDetector::VectorLengthOfFieldVector DependencyDetector::maxAPosteoriLevel0(const models::ValueIndex nameToPredict, const models::ColumnIndexValuePairVector &indexedFields, size_t maxToEvaluate) const
{
    using FieldsToProbabilityMap = map<ColumnIndexValuePairVector, double>;
    FieldsToProbabilityMap vectorLengthByField;
    VectorLengthOfFieldVector vectorized;

    auto classIndex = _cpt.fieldIndex(nameToPredict);
    auto prioriClassification = _cpt.classify(classIndex, {});
    for (auto field : indexedFields) {
        auto classification = _cpt.classify(classIndex, {field});
        if (classification.vectorLength() > prioriClassification.vectorLength()) {
            vectorLengthByField[{field}] = classification.vectorLength();

        }
        for (auto previous : vectorLengthByField) {
            auto v = previous.first;
            if (containerHas(v, field)) {
                continue;
            }
            v.push_back(field);
            auto t = _cpt.classify(classIndex, v);
            if (t.vectorLength() > previous.second) {
                 vectorLengthByField[v] = t.vectorLength();
            }
        }
    }
    std::copy(vectorLengthByField.begin(), vectorLengthByField.end(), std::back_inserter(vectorized));
    sort(vectorized.begin(), vectorized.end(), [](VecorLengthOfField const &left, VecorLengthOfField const &right) {
        return right.second < left.second;
    });
    return {vectorized.begin(), vectorized.size() > maxToEvaluate ? vectorized.begin() + static_cast<int>(maxToEvaluate) : vectorized.end()};
}

DependencyDetector::FieldLikelihoodVector DependencyDetector::maximizeLikelyhoods(const models::ColumnIndexValuePairVector &fields) const
{
    using FieldProbsMap = map<ColumnIndexValuePairVector, FieldLikelihood>;
    FieldProbsMap likelihoods;
    for (auto field : fields) {
        likelihoods[{field}] = {field, {}, _cpt.probability({field})};
        for (auto previous : likelihoods) {
            auto v = previous.first;
            if (containerHas(v, field)) continue;
            auto l = _cpt.conditionalProbability({field}, v);
            if (l > previous.second.probability) {
                v.push_back(field);
                likelihoods[v] = l;
            }
        }
    }
    FieldLikelihoodVector vectorized;
    copy(likelihoods.begin(), likelihoods.end(), std::back_inserter(vectorized));
    sort(vectorized.begin(), vectorized.end(), [](auto left, auto right) -> bool{ return left.second > right.second; });
    return vectorized;
}

void DependencyDetector::buildGraph(VecorLengthOfField const &base, FieldLikelihoodVector const &likelihoods) const
{
    models::BayesNet net;
    for(auto column : base.first) {
        BayesNetFragment fragment;
        fragment.node.field = {_cpt.fieldName(column.columnIndex), column.value};
    }
}

Probability DependencyDetector::likelihood(const models::ColumnIndexValuePair &k, const models::ColumnIndexValuePairVector &input) const
{
    return _cpt.conditionalProbability({k}, input);
}

models::Probability DependencyDetector::conditionalProbability(const models::ColumnIndexValuePair &classifier, const models::ColumnIndexValuePairVector &input)
{
    return _cpt.conditionalProbability({classifier}, input);
}

void DependencyDetector::eliminateZeroEvidence(ColumnIndexValuePairVector &values) const
{
    auto iterator = values.begin();
    while (iterator != values.end()) {
        if (_cpt.probability({*iterator}) == 0._p) {
            iterator = values.erase(iterator);
        } else {
            ++iterator;
        }
    }
}

ColumnNameValuePairVector DependencyDetector::indexedPairVectorToNamedPairVector(const models::ColumnIndexValuePairVector &values) const
{
    vector<ColumnNameValuePair> result;
    for (auto v : values) {
        result.push_back({_cpt.fieldName(v.columnIndex), v.value});
    }
    return result;
}

ColumnIndexValuePairVector DependencyDetector::namedPairVectorToIndexedPairVector(const models::ColumnNameValuePairVector &values) const
{
    ColumnIndexValuePairVector result;
    for (auto v : values) {
        result.push_back({_cpt.fieldIndex(v.columnName), v.value});
    }
    return result;
}

} // namespace services
} // namespace mimir
