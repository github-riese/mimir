#include "ProbabilityDistribution.h"

#include <algorithm>
#include <cmath>

#include "../helpers/helpers.h"

using std::pair;
using std::vector;

using std::copy;
using std::back_inserter;
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

ProbabilityDistribution::ProbabilityDistribution(std::vector<Probability> probabilities, std::vector<ValueIndex> classifiers)
{
    copy(probabilities.begin(), probabilities.end(), back_inserter(_probabilities));
    copy(classifiers.begin(), classifiers.end(), back_inserter(_classifiers));
    analyze();
}

ProbabilityDistribution::ProbabilityDistribution(std::map<ValueIndex, Probability> probabilities)
{
    for_each(probabilities.begin(), probabilities.end(), [this](auto const &pair) -> void{
        _classifiers.push_back(pair.first);
        _probabilities.push_back(pair.second);
    });
    analyze();
}

ProbabilityDistribution::ProbabilityDistribution(std::map<std::vector<ValueIndex>, Probability>)
{

}

double ProbabilityDistribution::vectorLength() const
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

ValueProbability ProbabilityDistribution::max() const
{
    return { _maxVal, _maxProb };
}

ValueProbability ProbabilityDistribution::min() const
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
    _vectorLength = sqrt(
                accumulate(_probabilities.begin(), _probabilities.end(), Probability(0), [](double accu, Probability const &p) -> double {
                    double v = p.value();
                    return accu + v*v;
                })
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

ValueProbabilityVector ProbabilityDistribution::distribution() const
{
    ValueProbabilityVector result;
    if (_classifiers.size() == 0) {
        return result;
    }
    auto prob = _probabilities.begin();
    auto name = _classifiers.begin();
    while (prob != _probabilities.end() && name != _classifiers.end()) {
        result.push_back({*(name++), *(prob++)});
    }
    return result;
}

Probability ProbabilityDistribution::probabilityOf(ValueIndex name) const
{
    auto idx = find_if(_classifiers.begin(), _classifiers.end(), [&name](auto val) -> bool{
        return val == name;
    });
    if (idx == _classifiers.end()) {
        return 0._p;
    }
    return *(_probabilities.begin() + std::distance(_classifiers.begin(), idx));
}

std::ostream &ProbabilityDistribution::dump(std::ostream &stream, services::NameResolver &resolver) const
{
    stream << "vectorLength: " << _vectorLength << std::endl;
    stream << "classes: " << _probabilities.size() << std::endl;
    auto prob = _probabilities.begin();
    auto name = _classifiers.begin();
    bool haveName = name != _classifiers.end();
    while (prob != _probabilities.end()) {
        if (haveName) {
            stream << resolver.nameFromIndex(*(name++)) << ": ";
        }
        stream << *prob;
        if (*prob == _maxProb) {
            stream << " (max)";
        } else if (*prob == _minProb) {
            stream << " (min)";
        }
        stream << std::endl;
        ++prob;
    }
    return stream;
}

vector<Probability> ProbabilityDistribution::probabilities() const
{
    return _probabilities;
}

} // namespace models
} // namespace mimir
