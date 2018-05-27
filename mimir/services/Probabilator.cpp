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
    _name(name)
{
}

void Probabilator::addSampler(const Sampler &sampler)
{
    _samplers.push_back(sampler);
}

Evaluation Probabilator::evaluate(ValueIndex value) const
{
    Evaluation result;
    map<ValueIndex, vector<Probability>> probabilities;
    for (auto sampler : _samplers) {
        for (auto classification : sampler.allClasses()) {
            if (!sampler.total() || !sampler.countInClass(classification)) {
                continue;
            }
            probabilities[classification].push_back(calculate(sampler, classification, value));
        }
    }
    for (auto classification : probabilities) {
        result.addProbability(classification.first, metaProbability(classification.second));
    }
    return result;
}

models::Probability Probabilator::metaProbability(const std::vector<models::Probability> &p) const
{
    unsigned long totalSamples = 0;
    unsigned long totalInClass = 0;
    unsigned long totalInValue = 0;
    unsigned long totalInClassAndValue = 0;
    vector<ValueIndex> usedSamplers;
    for (auto pN : p) {
        totalSamples += pN.totalSamples();
        totalInClass += pN.totalInClass();
        totalInValue += pN.totalInValue();
        totalInClassAndValue += pN.countInClassAndValue();
        auto const &pNSamplers = pN.samplers();
        usedSamplers.insert(usedSamplers.end(), pNSamplers.begin(), pNSamplers.end());
    }
    return Probability{
            bayes(totalInClassAndValue, totalInClass, totalInValue, totalSamples),
            totalSamples,
            totalInClass,
            totalInValue,
            totalInClassAndValue,
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
        total,
        countInClass,
        countInValue,
        countInClassInValue,
        { sampler.nameIndex() }
    };
}

} // namespace services
} // namespace mimir
