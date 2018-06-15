#include "Evaluator.h"

#include <numeric>

#include "../models/ProbabilityWithPriors.h"

using std::accumulate;
using std::move;

using std::string;
using std::tuple;
using std::vector;

using mimir::models::Probability;
using mimir::models::ProbabilityWithPriors;
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
    result.setValueProbability(valueProbability);
    if (classes.empty()) {
        classes = sampler.allClasses();
    }
    for (auto classification : classes) {
        if (!sampler.countInClass(classification) || valueProbability < 1e-10l) {
            result.addProbability(classification, 0, 0, 0);
            continue;
        }
        auto inClassCount = static_cast<long double>(sampler.countInClass(classification));
        auto classInValueCount = static_cast<long double>(sampler.count(classification, value));
        auto likelyhood = classInValueCount/inClassCount;
        auto classProbability = inClassCount/total;
        result.addProbability(classification,
                                  {(likelyhood * classProbability)/valueProbability},
                                  likelyhood,
                                  classProbability
                              );
        ++_opcount;
    }
    result.evaluate();
    return result;
}

models::Evaluation Evaluator::classify(const std::vector<models::Evaluation> &sources)
{
    if (sources.size() == 1)
        return sources[0];
    map<ValueIndex, vector<Probability>> combinableProbabilities;
    map<ValueIndex, Probability> classProbabilities;

    Probability evidence(0);
    for (auto evaluation : sources) {
        for (auto classification : evaluation.classifications()) {
            ProbabilityWithPriors p = evaluation.probabilityByClassificationEx(classification);
            evidence += p.likelyhood() * p.classProbability();
            combinableProbabilities[classification].push_back(p.likelyhood());
            classProbabilities[classification] = p.classProbability();
        }
    }
    Evaluation result(combineSamplerIDs(sources));
    result.setValueProbability(evidence);
    for (auto classification : combinableProbabilities) {
        result.addProbability(classification.first, combineProbabilities(classification.second, classProbabilities[classification.first], evidence));
    }
    result.evaluate();
    return result;
}

/**
 * @brief Evaluator::combineProbabilities
 * Create P(C|V1,...Vn) from P(C|V1), ..., P(C|Vn)
 * @param p P(C|V1), ..., P(C|Vn)
 * @return
 */
models::ProbabilityWithPriors Evaluator::combineProbabilities(const std::vector<models::Probability> &p, models::Probability classProbability, models::Probability evidence)
{

    if (p.size() == 0) {
        return ProbabilityWithPriors{};
    }
    if (p.size() < 2) {
        return {
            p.at(0).value(),
            classProbability.isZero() ? Probability{0} : (evidence * p.at(0))/classProbability,
            classProbability,
            evidence
        };
    }
    Probability joinedLikelyhood(1.l);
    for (auto pN : p) {
        if (!pN.valid() || pN.isZero()) {
            continue;
        }
        joinedLikelyhood *= pN;
        ++_opcount;
    }
    long double joinedProbability(0);
    if (!evidence.isZero()) {
        joinedProbability = classProbability * joinedLikelyhood/evidence;
        ++_opcount;
    }

    return ProbabilityWithPriors{
            joinedProbability,
            joinedLikelyhood,
            classProbability,
            evidence
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
