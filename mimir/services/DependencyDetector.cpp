#include "DependencyDetector.h"

#include <algorithm>
#include <deque>

#include "../models/Probability.h"
#include "../traits/Timing.h"

using std::deque;
using std::vector;

using std::find_if;

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

std::vector<models::ConditionalProbability> DependencyDetector::detectDependencies(vector<ValueIndex> const& values, ValueIndex classifiingColumn)
{
    // what i want:
    // P(a|b,c,d,e,f,...,z) * P(b|c,d,e,f,...,z) * ... * P(z)
    traits::VerboseTiming<std::chrono::microseconds> _timer("detectDependencies");
    auto fields = _cpt.fields();
    vector<ColumnIndexValuePair> independent;
    auto classifyingIndex = _cpt.fieldIndex(classifiingColumn);
    vector<ColumnIndexValuePair> candidates;
    for (auto field : fields) {
        auto idx = _cpt.fieldIndex(field);
        if (idx == classifyingIndex) {
            continue;
        }
        candidates.push_back({idx, values.at(static_cast<size_t>(idx))});
    }
    eliminateZeroEvidence(candidates);
    vector<ConditionalProbability> result;
    auto candidate = candidates.begin();
    while (candidate != candidates.end()) {
        auto partner = candidate + 1;
        while (partner != candidates.end()) {
            if (partner == candidate) {
                ++partner;
                continue;
            }
            Probability pThisGivenThat = conditionalProbability(*candidate, {*partner});
            Probability pThatGivenThis = conditionalProbability(*partner, {*(candidate)});
            Probability p1AndP2 = _cpt.probability({*candidate, *partner});
            Probability p1 = _cpt.probability({*candidate});
            Probability p2 = _cpt.probability({*partner});
            std::cerr << "P(" << candidate->value << '|' << partner->value << "): " << pThisGivenThat << ", "
                      << "P(" << partner->value << '|' << candidate->value << "): " << pThatGivenThis << ", "
                      << "P(" << partner->value << ',' << candidate->value << "): " << p1AndP2 << ", "
                      << "P(a): " << p1 << ", "
                      << "P(b): " << p2
                      << std::endl;
            if (abs(pThisGivenThat - p1) < .05_p || abs(pThatGivenThis - p2) < .05_p || pThisGivenThat == pThatGivenThis) {
                independent.push_back(*candidate);
                independent.push_back(*partner);
            }
            ++partner;
        }
        ++candidate;
    }
    return result;
}

vector<NetworkFragment> DependencyDetector::findSuitableGraph(const vector<ColumnNameValuePair> &input)
{
    deque<ColumnIndexValuePair> possibleFields;
    for (auto candidate : input) {
        possibleFields.push_back({_cpt.fieldIndex(candidate.columnName), candidate.value});
    }
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
    vector<ColumnIndexValuePair> unassigned;
    vector<ColumnIndexValuePair> currentWorkload;
    vector<NetworkFragment> net;
    ColumnIndexValuePair i1, i2;
    while (possibleFields.size() > 0) {
        i1 = possibleFields.front(); possibleFields.pop_front();
        Probability previousL, l;
        if (possibleFields.size() == 0) {
            l = likelihood(i1, {});
            net.push_back({{_cpt.fieldName(i1.columnIndex), i1.value}, {}, l});
            break;
        }
        while (possibleFields.size() > 0) {
            i2 = possibleFields.front(); possibleFields.pop_front();
            currentWorkload.push_back(i2);
            l = likelihood(i1, currentWorkload);
            if (l <= previousL) {
                auto v = currentWorkload.back();
                currentWorkload.pop_back();
                unassigned.push_back(v);
                continue;
            }
            previousL = l;
        }
        net.push_back({{_cpt.fieldName(i1.columnIndex), i1.value}, indexedPairVectorToNamedPairVector(currentWorkload), l});
        possibleFields.insert(possibleFields.end(), unassigned.begin(), unassigned.end());
        unassigned.clear();
    }
    return net;
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
