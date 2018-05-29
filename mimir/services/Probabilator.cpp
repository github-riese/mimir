#include "Probabilator.h"

#include <numeric>

using std::accumulate;
using std::move;

using std::string;
using std::vector;

using mimir::models::InvalidIndex;
using mimir::models::Probability;
using mimir::models::Sample;
using mimir::models::ValueIndex;
using mimir::models::Evaluation;

namespace mimir {
namespace services {

Probabilator::Probabilator(const string &name) :
    _name(name),
    _sampler(InvalidIndex())
{
}

void Probabilator::setSampler(const Sampler &sampler)
{
    _sampler = sampler;
}

Evaluation Probabilator::evaluateSampler(ValueIndex value) const
{
    Evaluation result;
    map<ValueIndex, vector<Probability>> probabilities;
    for (auto classification : _sampler.allClasses()) {
        if (!_sampler.total() || !_sampler.countInClass(classification)) {
            continue;
        }
        probabilities[classification].push_back(calculate(_sampler, classification, value));
    }
    for (auto classification : probabilities) {
        result.addProbability(classification.first, combineProbabilities(classification.second));
    }
    return result;
}

models::Probability Probabilator::combineProbabilities(const std::vector<models::Probability> &p) const
{
    if (p.size() < 2) {
        return p[0];
    }
    vector<vector<ValueIndex>> usedSamplers;
    long double combinedProbability = 1.L;
    long double combinedClassProbailities = 1.L;
    long double combinedValueProbailities = 1.L;
    for (auto pN : p) {
        if (!pN.valid() || pN.isZero()) {
            continue;
        }
        combinedProbability *= pN.probability();
        combinedClassProbailities *= pN.classProbability();
        combinedValueProbailities *= pN.valueProbability();
        auto pNSamplers = pN.samplers();
        usedSamplers.insert(usedSamplers.end(), pNSamplers.begin(), pNSamplers.end());
    }
    return Probability{
            bayes(combinedProbability, combinedClassProbailities, combinedValueProbailities),
            combinedClassProbailities,
            combinedValueProbailities,
            move(usedSamplers)
    };
}

Probability Probabilator::calculate(const Sampler &sampler, ValueIndex classification, ValueIndex value) const
{
    // P(class|value) = (P(value|class) * P(value))/(P(class))
    // P(value|class) = count of class in value / count in value
    // P(value) = count in value / count in total
    // P(class) = count in class / count in total
    unsigned long countInClassInValue = sampler.count(classification, value);
    unsigned long countInValue = sampler.countInValue(value);
    unsigned long countInClass = sampler.countInClass(classification);
    unsigned long total = sampler.total();
    return Probability{
            bayes(countInClassInValue, countInClass, countInValue, total),
            static_cast<long double>(countInClass)/static_cast<long double>(total),
            static_cast<long double>(countInValue)/static_cast<long double>(total),
            {{ sampler.nameIndex() }}
    };
}

long double Probabilator::bayes(unsigned long countInClassAndValue, unsigned long totalInClass, unsigned long totalInValue, unsigned long totalSamples) const
{
    return
            (
                (static_cast<long double>(countInClassAndValue) / static_cast<long double>(totalInClass)) *
                (static_cast<long double>(totalInClass) / static_cast<long double>(totalSamples))
                ) /
            (static_cast<long double>(totalInValue) / static_cast<long double>(totalSamples));
}

long double Probabilator::bayes(long double pBonConditionA, long double pA, long double pB) const
{
    return (pBonConditionA * pA) / pB;
}

} // namespace services
} // namespace mimir
