#include "MaxClassTurnOutDetector.h"

#include <algorithm>

#include "helpers/helpers.h"

using std::vector;
using std::pair;
using std::map;

using std::find_if;

using mimir::models::ColumnIndexValuePair;
using mimir::models::ColumnIndexValuePairVector;
using mimir::models::Probability;
using mimir::models::detect::InternalNode;
using mimir::models::detect::InternalFragment;
using mimir::models::detect::InternalFragmentVector;
using mimir::models::detect::InternalNet;
using mimir::models::detect::InternalNetVector;

using mimir::helpers::containerHas;

namespace mimir {
namespace services {
namespace detect {

using VectorLengthOfFields = pair<ColumnIndexValuePairVector, double>;

struct LengthSorter {
    bool operator()(VectorLengthOfFields const &left, VectorLengthOfFields const &right) const
    {
        return left.first.size() > right.first.size();
    }
};

struct AscSorter {
    bool operator()(VectorLengthOfFields const &left, VectorLengthOfFields const &right) const
    {
        return left.second < right.second;
    }
};

struct DescSorter {
    bool operator()(VectorLengthOfFields const &left, VectorLengthOfFields const &right) const
    {
        return right.second < left.second;
    }
};


MaxClassTurnOutDetector::MaxClassTurnOutDetector(models::CPT &cpt, long classFieldIndex, models::ColumnIndexValuePairVector const &sample) :
    AbstractDependencyDetector(cpt, classFieldIndex, sample)
{
}

models::detect::InternalFragmentVector MaxClassTurnOutDetector::detect(const models::ProbabilityDistribution &baseProbability)
{
    _baseDistribution = baseProbability;
    auto parents = findDirectParents();
    maximizeLikelyhoods();
    return _likelihoods;
}


InternalNet MaxClassTurnOutDetector::buildNet(const ColumnIndexValuePairVector &parents, const InternalFragmentVector &likelihoods)
{
    InternalNet net;
    for (auto &parent : parents) {
        auto lp = likelihoods.begin();
        for (;lp != likelihoods.end(); ++lp)
        {
            lp = find_if(lp, likelihoods.end(), [&parent](auto const &fragment) -> bool {return fragment.node.column.columnIndex == parent.columnIndex;});
            if (lp == likelihoods.end()) {
                break;
            }
            if (net.canAdd(*lp)) {
                net.parents.push_back(*lp);
                break;
            }
        }
    }
    return net;
}

std::vector<models::ColumnIndexValuePairVector> MaxClassTurnOutDetector::findDirectParents()
{
    vector<VectorLengthOfFields> vectorLengthsByField;
    auto parameters = _sampleFields;
    sort(parameters, false);
    for (auto field : parameters) {
        auto classification = _cpt.classify(_classFieldIndex, {field});
        vectorLengthsByField.push_back({{field}, classification.vectorLength()});
        for (auto previous = vectorLengthsByField.begin(); previous != vectorLengthsByField.end(); ++previous) {
            auto v = previous->first;
            if (containerHas(v, field)) {
                continue;
            }
            v.push_back(field);
            auto t = _cpt.classify(_classFieldIndex, v);
            if (t.vectorLength() > previous->second) {
                sort(v.begin(), v.end());
                previous = vectorLengthsByField.emplace(previous, v, t.vectorLength());
                ++previous;
            }
        }
        sort(vectorLengthsByField.begin(), vectorLengthsByField.end(), LengthSorter());
    }
    sort(vectorLengthsByField.begin(), vectorLengthsByField.end(), DescSorter());
    auto pick = vectorLengthsByField.begin();
    std::vector<models::ColumnIndexValuePairVector> baseParents;
    for (auto n = 0u; pick != vectorLengthsByField.end(); ++n, ++pick) {
        if (pick->second > _baseDistribution.vectorLength())
            baseParents.emplace_back(pick->first);
    }
    return baseParents;
}

void MaxClassTurnOutDetector::maximizeLikelyhoods()
{
    _likelihoods.clear();
    InternalFragmentVector fragmentVector;
    for (auto sample : _sampleFields) {
        auto p = _cpt.probability({sample});
        InternalNode field{sample, p};
        _likelihoods.push_back({field, {}});
        for (auto previous : _likelihoods) {
            auto v = previous.parentValues();
            if (previous.contains(sample)) continue;
            v.push_back(sample);
            auto l = _cpt.conditionalProbability({previous.node.column}, v);
            if (l > previous.node.probability) {
                previous.node.probability = l;
                previous.parents.push_back({{sample, p},{}});
                _likelihoods.push_back(previous);
            }
        }
    }
    sort(_likelihoods.begin(), _likelihoods.end(), [](auto left, auto right) -> bool{ return left.node.probability > right.node.probability; });
}

InternalFragmentVector MaxClassTurnOutDetector::likelihoodFragments()
{
    InternalFragmentVector fragments;
    for (auto likelihood : _likelihoods) {
        InternalFragment fragment;
    }
    return fragments;
}

bool MaxClassTurnOutDetector::buildInternalNet(models::detect::InternalNet &baseNet, models::detect::InternalFragmentVector &level, const models::detect::InternalFragment &toBeAdded)
{

}

} // namespace detect
} // namespace services
} // namespace mimir
