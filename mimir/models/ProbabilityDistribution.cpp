#include "ProbabilityDistribution.h"

#include <algorithm>
#include <cmath>

#include "../helpers/helpers.h"

using std::pair;
using std::vector;

using std::distance;
using std::for_each;
using std::max_element;
using std::min_element;
using std::sqrtl;

using mimir::helpers::variance;

namespace mimir {
namespace models {

ProbabilityDistribution::ProbabilityDistribution(vector<pair<ValueIndex, Probability> > probabilities)
{
    for_each(probabilities.begin(), probabilities.end(), [this](pair<ValueIndex, Probability> const &p){
        _classifiers.push_back(p.first);
        _probabilities.push_back(p.second);
    });
    analyze();
}

ProbabilityDistribution::ProbabilityDistribution(std::vector<Probability> probabilties, std::vector<ValueIndex> classifiers)
{
    for_each(probabilties.begin(), probabilties.end(), [this](Probability const &p){
        _probabilities.push_back(p);
    });

    for_each(classifiers.begin(), classifiers.end(), [this](ValueIndex const &v){
        _classifiers.push_back(v);
    });
    analyze();
}

ProbabilityDistribution::ProbabilityDistribution(std::map<ValueIndex, Probability> probabilities)
{
    for (auto p : probabilities) {
        _classifiers.push_back(p.first);
        _probabilities.push_back(p.second);
    }
    analyze();
}

Probability ProbabilityDistribution::vectorLength() const
{
    return _vectorLength;
}

Probability ProbabilityDistribution::argMax() const
{
    return _maxProb;
}

ValueIndex ProbabilityDistribution::mostProbable() const
{
    return _maxVal;
}

Probability ProbabilityDistribution::argMin() const
{
    return _minProb;
}

ValueIndex ProbabilityDistribution::leastProbable() const
{
    return _minVal;
}

pair<ValueIndex, Probability> ProbabilityDistribution::max() const
{
    return { _maxVal, _maxProb };
}

pair<ValueIndex, Probability> ProbabilityDistribution::min() const
{
    return { _minVal, _minProb };
}

bool ProbabilityDistribution::operator <(const ProbabilityDistribution &rhs) const
{
    return _vectorLength < rhs._vectorLength;
}

void ProbabilityDistribution::analyze()
{
    if (_probabilities.size() == 0) {
        return;
    }
    _vectorLength = sqrtl(
                _probabilities.size() * variance(_probabilities.begin(), _probabilities.end())
                        + 1/_probabilities.size()
            );
    auto element = max_element(_probabilities.begin(), _probabilities.end());
    _maxProb = *element;
    if (_classifiers.size() > 0) {
        _maxVal = *(_classifiers.begin() + distance(_probabilities.begin(), element));
    }
    element = min_element(_probabilities.begin(), _probabilities.end());
    _minProb = *element;
    if (_classifiers.size() > 0) {
        _minVal = *(_classifiers.begin() + distance(_probabilities.begin(), element));
    }
}

vector<ValueIndex> ProbabilityDistribution::classifiers() const
{
    return _classifiers;
}

vector<Probability> ProbabilityDistribution::probabilities() const
{
    return _probabilities;
}

} // namespace models
} // namespace mimir
