#include "EvaluationCombiner.h"

#include <algorithm>
#include <numeric>

#include "Evaluator.h"
#include "helpers/helpers.h"

using std::deque;
using std::multiplies;
using std::pair;
using std::vector;

using std::accumulate;
using std::find_if;
using std::for_each;
using std::sort;
using std::unique;

using mimir::models::Evaluation;
using mimir::models::Probability;
using mimir::models::ValueIndex;

namespace mimir {
namespace services {

EvaluationCombiner::EvaluationCombiner(DataStore &dataStore) :
    _dataStore(dataStore)
{

}

void EvaluationCombiner::addEvaluations(deque<Evaluation> newSets)
{
    _candidats.insert(_candidats.end(), newSets.begin(), newSets.end());
}

unsigned EvaluationCombiner::findClusters(int maxDepth)
{
    Evaluator e;
    _resulting.clear();
    if (-1 == maxDepth) {
        maxDepth = static_cast<int>(_candidats.size());
    }
    vector<ValueIndex> sampleProperties(_candidats.size());
    for_each(_candidats.begin(), _candidats.end(), [&sampleProperties](const Evaluation &e){
        sampleProperties.push_back(e.samplers().at(0).at(0));
    });
    _currentClassBuildIterator = sampleProperties.begin();
    _currentValueBuildIterator = _currentClassBuildIterator + 1;
    // for each _candidate:
    //   take its property pr_x and the property of its neighbour pr_y, then the next neighbour, ...
    //   make two samplers: pr_x|pr_y,  pr_y|pr_x
    //   evaluate the samplers by only one class (pr_*) P(pr_x|pr_y), P(pr_y|pr_x)
    //   combine evaluation of candidate * P(pr_x|pr_y) * P(property apriori)
    //   probability in one P(class|canditate) rises more than theta, then it's a cluster.
    return static_cast<unsigned>(maxDepth);
}

deque<Evaluation> EvaluationCombiner::result() const
{
    return _resulting;
}

pair<Sampler, Sampler> EvaluationCombiner::buildNextSubSampler()
{
    return {
        _dataStore.createSampler(*_currentClassBuildIterator, *_currentValueBuildIterator),
        _dataStore.createSampler(*_currentValueBuildIterator, *_currentClassBuildIterator)
    };
}

bool EvaluationCombiner::isSignificant(const models::Evaluation &combined, const std::vector<models::Evaluation> &sources) const
{
    std::vector<Probability> mostProbables = mimir::helpers::fetchMostProbables(sources);

    Probability mult = accumulate(mostProbables.begin(), mostProbables.end(), Probability(1), multiplies<Probability>());
    return combined.mostProbable() == mult;
}

} // namespace services
} // namespace mimir
