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

using mimir::models::BayesNet;
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
    LoggingTiming<std::chrono::microseconds> _timer(__PRETTY_FUNCTION__);
    vector<Node> priorNodes;
    for (auto value : input) {
        priorNodes.push_back({{value}, _cpt.probability({value})});
    }
    sort(priorNodes.begin(), priorNodes.end());
    return priorNodes;
}

NodeVector DependencyDetector::computePriors(const models::ColumnIndexValuePairVector &input) const
{
    // a list. for each input - P(xn=in)
    // Probabilities delievered as vector of Nodes
    // sorted by probability ascending
    LoggingTiming<std::chrono::microseconds> _timer(__PRETTY_FUNCTION__);
    vector<models::Node> priorNodes;
    for (auto value : input) {
        priorNodes.push_back({{_cpt.fieldName(value.columnIndex), value.value}, priorOf(value)});
    }
    sort(priorNodes.begin(), priorNodes.end());
    return priorNodes;
}

BayesNetFragment DependencyDetector::findPredictionGraph(const models::ValueIndex nameToPredict, const models::ColumnNameValuePairVector &input, size_t maxGrapths, Strategy strategy)
{
    _className = nameToPredict;
    _classIndex = _cpt.fieldIndex(_className);
    _examinedParams = namedPairVectorToIndexedPairVector(input);
    buildPriorMap();
    findAnyGraph(maxGrapths);
    return BayesNetFragment();
}

void DependencyDetector::buildPriorMap()
{
    auto priors = computePriors(_examinedParams);
    for(auto prior : priors)
        _priors[_cpt.fieldIndex(prior.field.columnName)] = prior;
}

BayesNetFragmentVector DependencyDetector::findAnyGraph(size_t maxToEvaluate)
{
    LoggingTiming<std::chrono::microseconds> _timer(__PRETTY_FUNCTION__);
    /* This is what we'll do:
     * 1. we get the likelihood of each parameter given any other parameter if the likelihood is greater than the probability of the parameter alone.
     * 2. we find the max a posteoris (whithout acutally classifiing!) given any combination of input. Of those we choose maxToEvaluate. This will be the root of the graph.
     * 3. for each result of 3 we now build a parameter tree that contains all the parameters.
     */
    maximizeLikelyhoods();
    auto graphBases = maxAPosteoriLevel0(maxToEvaluate);
    for (auto graphBase : graphBases)
    {
        buildGraph(graphBase, _likelihoods);
    }

    return {};
}

DependencyDetector::VectorLengthOfFieldVector DependencyDetector::maxAPosteoriLevel0(size_t maxToEvaluate) const
{
    using FieldsToProbabilityMap = map<ColumnIndexValuePairVector, double>;
    FieldsToProbabilityMap vectorLengthByField;
    VectorLengthOfFieldVector vectorized;

    auto prioriClassification = _cpt.classify(_classIndex, {});
    auto parameters = _examinedParams;
    sort(parameters.begin(), parameters.end(),
         [this](ColumnIndexValuePair const &left, ColumnIndexValuePair const &right){
            return _priors.at(left.columnIndex).probability > _priors.at(right.columnIndex).probability;
        }
    );
   for (auto field : parameters) {
        auto classification = _cpt.classify(_classIndex, {field});
        if (classification.vectorLength() > prioriClassification.vectorLength()) {
            vectorLengthByField[{field}] = classification.vectorLength();
        }
        for (auto previous : vectorLengthByField) {
            auto v = previous.first;
            if (containerHas(v, field)) {
                continue;
            }
            v.push_back(field);
            auto t = _cpt.classify(_classIndex, v);
            if (t.vectorLength() > previous.second) {
                sort(v.begin(), v.end());
                 vectorLengthByField[v] = t.vectorLength();
            }
        }
    }
    std::copy(vectorLengthByField.begin(), vectorLengthByField.end(), std::back_inserter(vectorized));
    sort(vectorized.begin(), vectorized.end(), [](VecorLengthOfField const &left, VecorLengthOfField const &right) {
        if (right.second == left.second)
            return right.first.size() < left.first.size();
        return right.second < left.second;
    });
    vector<BayesNet> baseNets;
    auto netbase = vectorized.begin();
    for (auto n = 0u; n < maxToEvaluate; ++n) {
        BayesNet net;
        for (auto parent : netbase->first) {
            net.parents.push_back({_priors.at(parent.columnIndex), {}});
        }
        baseNets.push_back(net);
    }
    return {vectorized.begin(), vectorized.size() > maxToEvaluate ? vectorized.begin() + static_cast<int>(maxToEvaluate) : vectorized.end()};
}

void DependencyDetector::maximizeLikelyhoods()
{
    _likelihoods.clear();
    for (auto field : _examinedParams) {
        _likelihoods.push_back({field, {}, _cpt.probability({field})});
        for (auto previous : _likelihoods) {
            auto v = previous.parents;
            if (previous.field == field || containerHas(v, field)) continue;
            v.push_back(field);
            auto l = _cpt.conditionalProbability({previous.field}, v);
            if (l > previous.probability) {
                _likelihoods.push_back({previous.field, v, l});
            }
        }
    }
    sort(_likelihoods.begin(), _likelihoods.end(), [](auto left, auto right) -> bool{ return left.probability > right.probability; });
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

Probability DependencyDetector::priorOf(models::ColumnIndexValuePair const &value) const
{
    return  _cpt.probability({value});
}

} // namespace services
} // namespace mimir
