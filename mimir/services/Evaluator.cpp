#include "Evaluator.h"

#include <numeric>

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
    sampler.total();
    if (!sampler.total()) {
        return result;
    }
    auto total = static_cast<long double>(sampler.total());
    auto inValueCount = static_cast<long double>(sampler.countInValue(value));

    long double valueProbability = inValueCount/total;
    for (auto classification : sampler.allClasses()) {
        if (!sampler.countInClass(classification)) {
            continue;
        }
        auto classInValueCount = static_cast<long double>(sampler.count(classification, value));
        auto inClassCount = static_cast<long double>(sampler.countInClass(classification));
        auto aPrioriA = classInValueCount/inClassCount;
        auto classProbability = inClassCount/total;
        result.addProbability(classification, Probability(
                                  (aPrioriA * classProbability)/valueProbability,
                                  classProbability,
                                  valueProbability,
                                  {{sampler.nameIndex()}}));
        ++_opcount;
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
            (combinedProbability*combinedClassProbailities) / combinedValueProbailities,
            combinedClassProbailities,
            combinedValueProbailities,
            move(usedSamplers)
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
