#include "Evaluator.h"

#include <numeric>

#include "../models/ProbabilityWithAPrioris.h"

using std::accumulate;
using std::move;

using std::string;
using std::tuple;
using std::vector;

using mimir::models::Probability;
using mimir::models::ProbabilityWithAPrioris;
using mimir::models::Sample;
using mimir::models::ValueIndex;
using mimir::models::Evaluation;

namespace mimir {
namespace services {

Evaluation Evaluator::evaluate(const Sampler &sampler, ValueIndex value, vector<ValueIndex> classes)
{
    Evaluation result({{sampler.nameIndex()}});
    sampler.total();
    if (!sampler.total()) {
        return result;
    }
    auto total = static_cast<long double>(sampler.total());
    auto inValueCount = static_cast<long double>(sampler.countInValue(value));

    long double valueProbability = inValueCount/total;
    if (classes.empty()) {
        classes = sampler.allClasses();
    }
    for (auto classification : classes) {
        if (!sampler.countInClass(classification) || valueProbability < 1e-10l) {
            result.addProbability(classification, { 0, 0, 0, valueProbability});
            continue;
        }
        auto inClassCount = static_cast<long double>(sampler.countInClass(classification));
        auto classInValueCount = static_cast<long double>(sampler.count(classification, value));
        auto likelyhood = classInValueCount/inClassCount;
        auto classProbability = inClassCount/total;
        result.addProbability(classification, {
                                  {(likelyhood * classProbability)/valueProbability},
                                  likelyhood,
                                  classProbability,
                                  valueProbability
                              });
        ++_opcount;
    }
    return result;
}

models::Evaluation Evaluator::evaluate(const std::vector<models::Evaluation> &sources, std::vector<ValueIndex> classes)
{
    if (sources.size() == 1)
        return sources[0];
    map<ValueIndex, vector<ProbabilityWithAPrioris>> combinableProbabilities;

    if (classes.empty()) {
        classes = sources.at(0).classifications();
    }
    for (auto evaluation : sources) {
        for (auto classification : evaluation.classifications()) {
            combinableProbabilities[classification].push_back(evaluation.probabilityByClassificationEx(classification));
        }
    }
    Evaluation result(combineSamplerIDs(sources));
    for (auto index : combinableProbabilities) {
        result.addProbability(index.first, combineProbabilities(index.second));
    }
    return result;
}

/**
 * @brief Evaluator::combineProbabilities
 * Create P(C|V1,...Vn) from P(C|V1), ..., P(C|Vn)
 * @param p P(C|V1), ..., P(C|Vn)
 * @return
 */
models::ProbabilityWithAPrioris Evaluator::combineProbabilities(const std::vector<models::ProbabilityWithAPrioris> &p)
{

    if (p.size() < 2) {
        return p[0];
    }
    long double mod = static_cast<long double>(p.size());
    long double combinedLikelyhood = 1.L;
    long double combinedClassProbabilities = 0.L;
    long double combinedValueProbabilities = 0.L;
    for (auto pN : p) {
        if (!pN.probability().valid() || pN.probability().isZero()) {
            continue;
        }
        combinedLikelyhood *= pN.likelyHood().value();
        combinedClassProbabilities += pN.classProbability().value();
        combinedValueProbabilities += (pN.classProbability() * pN.likelyHood()).value();
    }
    long double combinedProbability = 1.L;
    combinedClassProbabilities /= mod;
    combinedProbability = 1.l/combinedValueProbabilities * combinedClassProbabilities * combinedLikelyhood;

    combinedClassProbabilities /= mod;

    return ProbabilityWithAPrioris{
            combinedProbability,
            combinedLikelyhood,
            combinedClassProbabilities,
            combinedValueProbabilities
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

vector<vector<ValueIndex>> Evaluator::combineSamplerIDs(const vector<Evaluation> &evaluations)
{
    vector<vector<ValueIndex>> combined;
    for (auto e : evaluations) {
        combined.push_back(combineSamplerIDs(e.samplers()));
    }
    return combined;
}

} // namespace services
} // namespace mimir
