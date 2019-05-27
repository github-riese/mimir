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

models::detect::InternalNetVector MaxClassTurnOutDetector::buildNets(const models::ProbabilityDistribution &baseProbability, size_t maxResults)
{
    _baseDistribution = baseProbability;
    auto parents = findDirectParents(maxResults);
    auto likelihoods = maximizeLikelyhoods();

    InternalNetVector internalNets;
    for (auto const &parent : parents)
    {
        InternalNet net = buildNet(parent, likelihoods);
        internalNets.push_back(net);
    }
    return internalNets;
}

InternalNet MaxClassTurnOutDetector::buildNet(const ColumnIndexValuePairVector &parents, const InternalFragmentVector &likelihoods)
{
    InternalNet net;

    for (auto &parent : parents) {
        auto lp = likelihoods.begin();
        while (lp != likelihoods.end())
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

std::vector<models::ColumnIndexValuePairVector> MaxClassTurnOutDetector::findDirectParents(size_t maxResults)
{
    if (maxResults == 0)
        return {};
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
    for (auto n = 0u; n < maxResults && pick != vectorLengthsByField.end(); ++n, ++pick) {
        if (pick->second > _baseDistribution.vectorLength())
            baseParents.emplace_back(pick->first);
    }
    return baseParents;
}

models::detect::InternalFragmentVector MaxClassTurnOutDetector::maximizeLikelyhoods()
{
    _likelihoods.clear();
    InternalFragmentVector fragmentVector;
    map<ColumnIndexValuePair, Probability> fieldProbs;
    for (auto field : _sampleFields) {
        auto p = _cpt.probability({field});
        fieldProbs[field] = p;
        _likelihoods.push_back({field, {}, p});
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
    for (auto const &l : _likelihoods) {
        InternalFragment f{{l.field, fieldProbs[l.field]}, {}};
        for(auto const &p : l.parents) {
            f.parents.push_back({{p, fieldProbs[p]},{}});
        }
        fragmentVector.push_back(f);
    }
    return fragmentVector;
}

} // namespace detect
} // namespace services
} // namespace mimir
