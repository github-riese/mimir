#include "DependencyDetector.h"

#include <algorithm>
#include <deque>
#include <map>
#include <vector>

#include <iostream>

#include <iotaomegapsi/tools/logger/Logger.h>

#include "detect/DependencyDetectionInterface.h"

#include "../helpers/helpers.h"
#include "../models/BayesNetFragment.h"
#include "../models/detect/InternalNodeTree.h"
#include "../models/Probability.h"
#include <iotaomegapsi/tools/timer/Timing.h>

#include "detect/DetectorFactory.h"

using std::back_inserter;
using std::deque;
using std::pair;
using std::map;
using std::vector;
using std::min;

using std::copy;
using std::find_if;
using std::sort;
using std::fabs;

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
using mimir::models::ProbabilityDistribution;
using mimir::models::ValueIndex;

using namespace mimir::models::detect;

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

models::BayesNet DependencyDetector::findPredictionGraph(const models::ValueIndex nameToPredict, const models::ColumnNameValuePairVector &input, size_t maxGraphs, models::detect::DetectStrategy strategy)
{
    LoggingTiming<std::chrono::microseconds> _timer(__PRETTY_FUNCTION__);
    _className = nameToPredict;
    _classIndex = _cpt.fieldIndex(_className);
    _examinedParams = namedPairVectorToIndexedPairVector(input);
    auto classDistribution = _cpt.classify(_classIndex, {});
    detect::DetectorFactory detectorFactory(_cpt, _classIndex, _examinedParams);
    detect::SharedDetector maxTurnoutDetector = detectorFactory.getDetector(strategy);
    auto internalNets = maxTurnoutDetector->buildNets(classDistribution, maxGraphs);
    return convert(internalNets.front(), classDistribution, _cpt);
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
    return {};
}

bool closeEnough(double left, double right)
{
    return fabs(left - right) * 1e7 < min(fabs(left), fabs(right));;
}



void DependencyDetector::buildGraph(const models::ColumnIndexValuePairVector &base) const
{
    InternalNet net;
    for (auto parent : base) {
        net.parents.push_back({{parent, _priors.at(parent.columnIndex).probability}, {}});
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
