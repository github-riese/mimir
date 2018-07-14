#include "DependencyDetector.h"

#include <algorithm>
#include <deque>

#include "../models/Network.h"
#include "../models/Probability.h"
#include "../traits/Timing.h"

using std::deque;
using std::vector;

using std::find_if;
using std::sort;

using mimir::models::CPT;
using mimir::models::ColumnIndexValuePair;
using mimir::models::ColumnNameValuePair;
using mimir::models::ConditionalProbability;
using mimir::models::NamedProbability;
using mimir::models::NetworkFragment;
using mimir::models::Probability;
using mimir::models::ValueIndex;

namespace mimir {
namespace services {

DependencyDetector::DependencyDetector(CPT &cpt) :
    _cpt(cpt)
{
}

vector<NetworkFragment> DependencyDetector::computePriors(vector<ColumnIndexValuePair> const& input)
{
    // a list. for each input - P(xn=in)
    // Probabilities delievered as vector of NetworkFragments
    // sorted by probability ascending
    traits::VerboseTiming<std::chrono::microseconds> _timer(__PRETTY_FUNCTION__);
    vector<NetworkFragment> result;
    for (auto value : input) {
        Probability p = _cpt.probability({value});
        result.push_back(NetworkFragment{{_cpt.fieldName(value.columnIndex), value.value}, {}, p});
    }
    sort(result.begin(), result.end());
    return result;
}
/*
    auto candidate = candidates.begin();
    while (candidate != candidates.end()) {
        auto partner = candidate + 1;
        while (partner != candidates.end()) {
            if (partner == candidate) {
                ++partner;
                continue;
            }
            Probability p = conditionalProbability(*candidate, {*partner});
            result.push_back({{_cpt.fieldName(candidate->columnIndex), candidate->value}, {{_cpt.fieldName(partner->columnIndex), partner->value}}, p});
            p = conditionalProbability(*partner, {*candidate});
            result.push_back({{_cpt.fieldName(partner->columnIndex), partner->value}, {{_cpt.fieldName(candidate->columnIndex), candidate->value}}, p});
            ++partner;
        }
        ++candidate;
    }
    std::sort(result.begin() + static_cast<long>(candidates.size()), result.end());
    return result;
}
*/

vector<NetworkFragment> DependencyDetector::findSuitableGraph(const vector<ColumnNameValuePair> &input)
{
    traits::VerboseTiming<std::chrono::microseconds> graphTiming(__PRETTY_FUNCTION__);
    vector<ColumnIndexValuePair> possibleFields;
    for (auto candidate : input) {
        possibleFields.push_back({_cpt.fieldIndex(candidate.columnName), candidate.value});
    }
    auto priors = computePriors(possibleFields);
    // look at first field x1. get probability P(x1=i1)
    // take the next field x2. get probability P(x2=i2)
    // get conditional probability P(x1=i1|x2=i2)
    // is P(x1=i1|x2=i2) equal P(x1=i1)? -> independend -> out
    // get the conditional probability P(x2=i2|x1=i1)
    // is P(x2=i2|x1=i1) equal P(x2=i2)? -> independend -> out
    // use the greater value for next step
    // take the next value, get the conditional Probability P(xA=iA|xB=iB,x3=i3)
    // is that one greater than the value before? take the next value as condition
    // is that one less than the value before? ignore x3, go on with x4
    // no more values left? the current one is one net fragment.
    // restart with all values that are not in the net.
    vector<ColumnIndexValuePair> potentialParents;
    models::Network n;
    ColumnIndexValuePair i1, i2;
    for (auto prior : priors) {
        for (size_t field = 0; field < possibleFields.size(); ++field) {
            Probability currentLikelihood = prior.probability(), l;
            ColumnNameValuePair const &priorInput = prior.input();
            i1 = {_cpt.fieldIndex(priorInput.columnName), priorInput.value};
            if (i1.columnIndex == possibleFields.at(field).columnIndex) {
                continue;
            }
            for (size_t parentField = 0; parentField < possibleFields.size(); ++parentField) {
                size_t parentIndex = field + parentField;
                if (parentIndex >= possibleFields.size()) {
                    parentIndex -= possibleFields.size(); // wrap around;
                }
                if (i1.columnIndex == possibleFields.at(parentIndex).columnIndex) {
                    continue;
                }
                i2 = possibleFields.at(parentIndex);
                potentialParents.push_back(i2);
                l = likelihood(i1, potentialParents);
                if (l <= currentLikelihood) {
                    potentialParents.pop_back();
                    continue;
                } else if (l == 1._p) {
                    currentLikelihood = l;
                    break;
                }
                currentLikelihood = l;
            }
            n.addFragment({{_cpt.fieldName(i1.columnIndex), i1.value}, indexedPairVectorToNamedPairVector(potentialParents), currentLikelihood});
            potentialParents.clear();
        }
    }
    return n.fragments();
}

Probability DependencyDetector::likelihood(const models::ColumnIndexValuePair &k, const std::vector<models::ColumnIndexValuePair> &x)
{
    if (x.size() == 0) {
        return _cpt.probability({k});
    }
    auto distribution = _cpt.classify(x, k.columnIndex);
    return distribution.probabilityOf(k.value);
}

models::Probability DependencyDetector::conditionalProbability(const models::ColumnIndexValuePair &classifier, const vector<ColumnIndexValuePair> &fields)
{
    auto result = _cpt.classify(fields, classifier.columnIndex);
    auto classes = result.distribution();
    auto v = find_if(classes.begin(), classes.end(), [&classifier](const NamedProbability& p){
        return p.name == classifier.value;
    });
    return v == classes.end() ? 0._p : (*v).probability;
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

vector<ColumnNameValuePair> DependencyDetector::indexedPairVectorToNamedPairVector(const vector<ColumnIndexValuePair> &values) const
{
    vector<ColumnNameValuePair> result;
    for (auto v : values) {
        result.push_back({_cpt.fieldName(v.columnIndex), v.value});
    }
    return result;
}

vector<ColumnIndexValuePair> DependencyDetector::namedPairVectorToIndexedPairVector(const vector<ColumnNameValuePair> &values) const
{
    vector<ColumnIndexValuePair> result;
    for (auto v : values) {
        result.push_back({_cpt.fieldIndex(v.columnName), v.value});
    }
    return result;
}

} // namespace services
} // namespace mimir
