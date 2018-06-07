#include "EvaluationCombiner.h"

#include "Evaluator.h"

using std::deque;
using mimir::models::Evaluation;

namespace mimir {
namespace services {

EvaluationCombiner::EvaluationCombiner()
{

}

void EvaluationCombiner::addEvaluations(deque<Evaluation> newSets)
{
    _candidats.insert(_candidats.end(), newSets.begin(), newSets.end());
}

unsigned EvaluationCombiner::findClusters(unsigned maxDepth)
{
    Evaluator e;
    deque<Evaluation> intermeditate = _candidats;
    unsigned depth = 0;
    for (; depth < maxDepth; ++depth) {
        deque<Evaluation> combined;
        auto running = intermeditate.begin();
        Evaluation parent1 = *running;
        ++running;
        bool foundSomething = false;
        combined.push_back(parent1);
        auto n = 0;
        while (running != intermeditate.end()) {
            Evaluation combine = e.evaluate({parent1, *running});
            auto pC = combine.mostProbable();
            auto dC = fabsl(pC - combine.mean());
            auto singleDeviation = ((parent1.deviation() + running->deviation()))/2;
            if (dC > singleDeviation) {
                combined.push_back(combine);
                foundSomething = true;
            } else {
                combined.push_back(*running);
            }
            ++running;
            ++n;
        }
        if (foundSomething) {
            combined.pop_front();
        }
        if (intermeditate.size() == combined.size()) {
            break;
        }
        intermeditate = combined;
    }
    _resulting = intermeditate;
    return depth + 1;
}

deque<Evaluation> EvaluationCombiner::result() const
{
    return _resulting;
}

} // namespace services
} // namespace mimir
