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
using mimir::models::NamedProbability;
using mimir::models::Network;
using mimir::models::NetworkFragment;
using mimir::models::Probability;
using mimir::models::ValueIndex;

namespace mimir {
namespace services {

DependencyDetector::DependencyDetector(CPT &cpt) :
    _cpt(cpt)
{
}

std::vector<models::NetworkFragment> DependencyDetector::computePriors(const std::vector<models::ColumnNameValuePair> &input) const
{
    return computePriors(namedPairVectorToIndexedPairVector(input));
}

vector<NetworkFragment> DependencyDetector::computePriors(vector<ColumnIndexValuePair> const& input) const
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

Network DependencyDetector::findSuitableGraph(const vector<ColumnNameValuePair> &input, NameResolver &nr)
{
    traits::VerboseTiming<std::chrono::microseconds> graphTiming(__PRETTY_FUNCTION__);
    auto likelyGraphs = findLikelyGraphs(input);
    auto bestGraphs = findBestGraphs(likelyGraphs, nr);
    return bestGraphs;
}

std::vector<models::NetworkFragment> DependencyDetector::findLikelyGraphs(const std::vector<models::ColumnNameValuePair> &input) const
{
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
    Network n;
    ColumnIndexValuePair i1, i2;
    for (auto prior : priors) {
        for (size_t field = 0; field < possibleFields.size(); ++field) {
            vector<ColumnIndexValuePair> potentialParents;
            Probability currentLikelihood = prior.probability(), l;
            ColumnNameValuePair const &priorInput = prior.input();
            i1 = {_cpt.fieldIndex(priorInput.columnName), priorInput.value};
            if (i1.columnIndex == possibleFields.at(field).columnIndex) {
                continue;
            }
            for (size_t parentField = 0; parentField < possibleFields.size(); ++parentField) {
                size_t parentIndex = field + parentField;
                if (parentIndex >= possibleFields.size()) {
                    parentIndex -= possibleFields.size(); // wrap around
                }
                if (i1.columnIndex == possibleFields.at(parentIndex).columnIndex) {
                    continue;
                }
                i2 = possibleFields.at(parentIndex);
                potentialParents.push_back(i2);
                l = likelihood(i1, potentialParents);
                if (l <= currentLikelihood) { // less likely is uninterestring, equal likelihood would point to independence
                    potentialParents.pop_back();
                    continue;
                }
                currentLikelihood = l;
                if (currentLikelihood == 1._p) {
                    break;
                }
            }
            n.addFragment({{_cpt.fieldName(i1.columnIndex), i1.value}, indexedPairVectorToNamedPairVector(potentialParents), currentLikelihood});
        }
    }
    return n.fragments();
}

Network DependencyDetector::findBestGraphs(const std::vector<NetworkFragment> &candidates, NameResolver& nr)
{
    vector<NetworkFragment> mostProbable = candidates;
    sort(mostProbable.begin(), mostProbable.end(), [](NetworkFragment const &left, NetworkFragment const &right){
        if( left.probability() > right.probability() )
            return true;
        if (left.probability() == right.probability())
            return left.countParents() > right.countParents();
        return false;
    });
    ValueIndex baseFieldName = mostProbable.front().input().columnName;
    vector<Network> networks;
    for (auto fragment : mostProbable) {
        if (fragment.input().columnName == baseFieldName) {
            Network n;
            n.addFragment(fragment);
            networks.push_back(n);
            continue;
        }
        bool added = false;
        auto currentNodeName = fragment.input().columnName;
        for (auto &net : networks) {
            if (!net.isKnownChild(currentNodeName) && net.canAdd(fragment)) {
                net.addFragment(fragment);
                added = true;
                break;
            }
        }
        if (!added) {
            Network n;
            n.addFragment(fragment);
            networks.push_back(n);
        }
    }
    auto nw = networks.begin();
    auto deepest = nw;
    int depth = 0;
    for (; nw != networks.end(); ++nw) {
        int d = (*nw).greatestDepth();
        if (d > depth) {
            depth = d;
            deepest = nw;
        }
    }
    return *deepest;
}

Probability DependencyDetector::likelihood(const models::ColumnIndexValuePair &k, const std::vector<models::ColumnIndexValuePair> &input) const
{
    if (input.size() == 0) {
        return _cpt.probability({k});
    }
    auto distribution = _cpt.classify(k.columnIndex, input);
    return distribution.probabilityOf(k.value);
}

models::Probability DependencyDetector::conditionalProbability(const models::ColumnIndexValuePair &classifier, const vector<ColumnIndexValuePair> &input)
{
    auto result = _cpt.classify(classifier.columnIndex, input);
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
