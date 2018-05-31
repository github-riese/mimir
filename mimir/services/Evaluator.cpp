#include "Evaluator.h"

#include <numeric>

#include "BayesCalculator.h"

using std::accumulate;
using std::move;

using std::string;
using std::vector;

using mimir::models::Probability;
using mimir::models::Sample;
using mimir::models::ValueIndex;
using mimir::models::Evaluation;

namespace mimir {
namespace services {

Evaluation Evaluator::evaluate(const Sampler &sampler, ValueIndex value)
{
    Evaluation result;
    map<ValueIndex, vector<Probability>> probabilities;
    for (auto classification : sampler.allClasses()) {
        if (!sampler.total() || !sampler.countInClass(classification)) {
            continue;
        }
        result.addProbability(classification, calculate(sampler, classification, value));
    }
    return result;
}

models::Evaluation Evaluator::evaluate(const std::vector<models::Evaluation> &sources)
{
    if (sources.size() == 1)
        return sources[0];
    map<ValueIndex, vector<Probability>> combinableProbabilities;
    for (auto evaluation : sources) {
        for (auto classification : evaluation.classifications()) {
            combinableProbabilities[classification].push_back(evaluation.probabilityByClassification(classification));
        }
    }
    Evaluation result;
    for (auto index : combinableProbabilities) {
        result.addProbability(index.first, combineProbabilities(index.second));
    }
    return result;
}

models::Probability Evaluator::combineProbabilities(const std::vector<models::Probability> &p)
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
        usedSamplers.push_back(combineSamplerIDs(pN.samplers()));
    }
    return Probability{
            BayesCalculator::calculate(combinedProbability, combinedClassProbailities, combinedValueProbailities),
            combinedClassProbailities,
            combinedValueProbailities,
            move(usedSamplers)
    };
}

Probability Evaluator::calculate(const Sampler &sampler, ValueIndex classification, ValueIndex value)
{
    // P(class|value) = (P(value|class) * P(value))/(P(class))
    // P(value|class) = count of class in value / count in value
    // P(value) = count in value / count in total
    // P(class) = count in class / count in total
    unsigned long countInClassInValue = sampler.count(classification, value);
    unsigned long countInValue = sampler.countInValue(value);
    unsigned long countInClass = sampler.countInClass(classification);
    unsigned long total = sampler.total();
    ++_opcount;
    return Probability{
            BayesCalculator::calculate(countInClassInValue, countInClass, countInValue, total),
            static_cast<long double>(countInClass)/static_cast<long double>(total),
            static_cast<long double>(countInValue)/static_cast<long double>(total),
            {{ sampler.nameIndex() }}
    };
}

std::vector<models::ValueIndex> Evaluator::combineSamplerIDs(const std::vector<std::vector<models::ValueIndex> > &sources)
{
    vector<ValueIndex> combined;
    for (auto source : sources) {
        combined.insert(combined.end(), source.begin(), source.end());
    }
    return combined;
}

} // namespace services
} // namespace mimir
