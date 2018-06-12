#include "Evaluation.h"

#include <algorithm>
#include <numeric>

#include "../helpers/helpers.h"

using std::pair;
using std::vector;

using std::fabsl;
using std::accumulate;
using std::for_each;
using std::max_element;

namespace mimir {

namespace models {

Evaluation::Evaluation(std::vector<std::vector<ValueIndex>> samplers) :
    _samplers(samplers)
{
}


void Evaluation::addProbability(ValueIndex classification, const ProbabilityWithAPrioris &probability)
{
    _probabilities[classification] = { probability.probability(), probability.likelyHood(), probability.probOfClass() };
    if (!_valueProbability.valid()) {
        _valueProbability = probability.probOfValue();
    }
    evaluate();
}

void Evaluation::addProbability(ValueIndex classification, const Probability &probability, const Probability &likelyhood, const Probability &classProbability)
{
    addProbability(classification, {probability, likelyhood, classProbability, _valueProbability});
}

std::vector<ValueIndex> Evaluation::classifications() const
{
    vector<ValueIndex> classifications;
    for (auto element : _probabilities) {
        classifications.push_back(element.first);
    }
    return  classifications;
}

bool Evaluation::operator<(const Evaluation &rhs) const
{
    if (_probabilities.size() < rhs._probabilities.size())
        return true;
    if (rhs._probabilities.empty()) {
        return false;
    }
    if (_probabilities.empty()) {
        return true;
    }
    return _probabilities.at(_mostProbable).p < rhs._probabilities.at(rhs._mostProbable).p;
}

bool Evaluation::operator==(const Evaluation &rhs) const
{
    return _probabilities == rhs._probabilities;
}

void Evaluation::evaluate()
{
    if (_probabilities.size() == 1) {
        auto ptr = _probabilities.begin();
        _mean = (*ptr).second.p;
        _deviation = 0.L;
        _mostProbable = (*ptr).first;
        return;
    }

    auto max = max_element(_probabilities.begin(), _probabilities.end(), [](const pair<ValueIndex, ProbabilityEx> &left, const pair<ValueIndex, ProbabilityEx> &right){
        return left.second.p < right.second.p;
    });

    _mostProbable = max->first;
    vector<Probability> pS;
    for_each(_probabilities.begin(), _probabilities.end(),
             [&pS](pair<ValueIndex, ProbabilityEx> const &item) {
        pS.push_back(item.second.p);
    });

    _mean = helpers::mean(pS);
    _deviation = helpers::deviation(_mean, pS);
}

bool Evaluation::ProbabilityEx::operator ==(const Evaluation::ProbabilityEx &rhs) const
{
    return p == rhs.p && classP == rhs.classP;
}

} // namespace models
} // namespace mimir
