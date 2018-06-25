#include "Evaluator.h"

#include <numeric>

#include "../models/ProbabilityWithPriors.h"

using std::accumulate;
using std::move;

using std::string;
using std::tuple;
using std::vector;
using std::map;

using mimir::models::Probability;
using mimir::models::ProbabilityWithPriors;
using mimir::models::ValueIndex;
using mimir::models::Evaluation;

namespace mimir {
namespace services {

models::Evaluation Evaluator::classify(const std::vector<models::Evaluation> &sources)
{
    if (sources.size() == 1)
        return sources[0];
    map<ValueIndex, vector<Probability>> likelihoods;
    map<ValueIndex, Probability> classPriors;

    Probability evidence(1);
    for (auto evaluation : sources) {
        for (auto classification : evaluation.classifications()) {
            ProbabilityWithPriors p = evaluation.probabilityByClassificationEx(classification);
            likelihoods[classification].push_back(p.likelihood());
            classPriors[classification] = p.prior();
        }
        evidence *= evaluation.evidence();
    }
    Evaluation result(combineSamplerIDs(sources));
    result.setEvidence(evidence);
    for (auto classification : likelihoods) {
        result.addProbability(classification.first, combineProbabilities(classification.second, classPriors[classification.first], evidence));
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
models::ProbabilityWithPriors Evaluator::combineProbabilities(const std::vector<models::Probability> &p, models::Probability classPrior, models::Probability evidence)
{

    if (p.size() == 0) {
        return ProbabilityWithPriors{};
    }
    if (p.size() < 2) {
        return {
            p.at(0).value(),
            classPrior.isZero() ? Probability{0} : (classPrior * p.at(0))/evidence,
            classPrior,
            evidence
        };
    }
    Probability joinedLikelyhood(1);
    for (auto pN : p) {
        joinedLikelyhood *= pN;
        ++_opcount;
    }
    long double joinedProbability(0);
    if (!evidence.isZero()) {
        joinedProbability = classPrior * joinedLikelyhood/evidence;
        ++_opcount;
    }

    return ProbabilityWithPriors{
            joinedProbability,
            joinedLikelyhood,
            classPrior,
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
