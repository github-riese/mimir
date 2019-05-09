#include "DependencyDetector.h"

#include <algorithm>
#include <deque>
#include <map>

#include <iostream>

#include <Logger.h>

#include "../helpers/helpers.h"
#include "../models/BayesNetFragment.h"
#include "../models/Probability.h"
#include "../traits/Timing.h"

using std::deque;
using std::pair;
using std::map;
using std::vector;

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
    traits::VerboseTiming<std::chrono::microseconds> _timer(__PRETTY_FUNCTION__);
    vector<models::Node> priorNodes;
    for (auto value : input) {
        Probability p = _cpt.probability({value});
        priorNodes.push_back({{_cpt.fieldName(value.columnIndex), value.value}, p});
    }
    sort(priorNodes.begin(), priorNodes.end());
    return priorNodes;
}

BayesNetFragment DependencyDetector::findPredictionGraph(const models::ValueIndex nameToPredict, const models::ColumnNameValuePairVector &input, NameResolver &nr)
{
    findAnyGraph(nameToPredict, input, nr);
    return BayesNetFragment();
}

BayesNetFragmentVector DependencyDetector::findAnyGraph(const models::ValueIndex nameToPredict, const models::ColumnNameValuePairVector &input, NameResolver &nr)
{
    traits::VerboseTiming<std::chrono::microseconds> _timer(__PRETTY_FUNCTION__);
    // we'll proceed this way:
    // we take the first input and calculate a distribution over nameToPredict and measure the vector length
    // then we take the next input parameter and calculate the distribution and measure the vector length of the distribution
    // if the vector is shorter now, we check param 2 alone
    // if it's still shorter than before number 2 is ot
    // if it's longer, it's in
    auto inputSize = input.size();
    typedef map<ColumnIndexValuePairVector, double> FieldsToProbabilityMap;
    FieldsToProbabilityMap vectorLengthByField;
    auto indexedFields = namedPairVectorToIndexedPairVector(input);
    auto classIndex = _cpt.fieldIndex(nameToPredict);
    auto prioriClassification = _cpt.classify(classIndex, {});
    for (auto field : indexedFields) {
        auto classification = _cpt.classify(classIndex, {field});
        if (classification.vectorLength() > prioriClassification.vectorLength()) {
            vectorLengthByField[{field}] = classification.vectorLength();

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
    }
    typedef pair<ColumnIndexValuePairVector, double> FieldsAndVlengths;
    vector<FieldsAndVlengths> vectorized;
    std::copy(vectorLengthByField.begin(), vectorLengthByField.end(), std::back_inserter(vectorized));
    sort(vectorized.begin(), vectorized.end(), [](FieldsAndVlengths const &left, FieldsAndVlengths const &right) {
        return right.second < left.second;
    });
    logger::Logger logger;
    logger << "Hello" << "world";
    cerr << "Fields and probs:" << endl;
    for (auto item : vectorized) {
        for (auto field : item.first) {
            cerr << nr.nameFromIndex(_cpt.fieldName(field.columnIndex)) << ", ";
        }
        cerr << " --> " << item.second << endl;
    }
    return {};
}


Probability DependencyDetector::likelihood(const models::ColumnIndexValuePair &k, const models::ColumnIndexValuePairVector &input) const
{
    if (input.size() == 0) {
        return _cpt.probability({k});
    }
    auto distribution = _cpt.classify(k.columnIndex, input);
    return distribution.probabilityOf(k.value);
}

models::Probability DependencyDetector::conditionalProbability(const models::ColumnIndexValuePair &classifier, const models::ColumnIndexValuePairVector &input)
{
    auto result = _cpt.classify(classifier.columnIndex, input);
    auto classes = result.distribution();
    auto v = find_if(classes.begin(), classes.end(), [&classifier](const models::NamedProbability& p){
        return p.first == classifier.value;
    });
    return v == classes.end() ? 0._p : (*v).second;
}

void DependencyDetector::eliminateZeroEvidence(std::vector<models::ColumnIndexValuePair> &values) const
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
