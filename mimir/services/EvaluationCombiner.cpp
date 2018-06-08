#include "EvaluationCombiner.h"

#include <algorithm>

#include "Evaluator.h"

using std::deque;
using std::find_if;
using std::for_each;
using std::sort;
using std::unique;

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

unsigned EvaluationCombiner::findClusters(int maxDepth)
{
    Evaluator e;
    _resulting.clear();
    if (-1 == maxDepth) {
        maxDepth = static_cast<int>(_candidats.size());
    }
    deque<Evaluation> result;
    deque<Evaluation> picked;
    int depth = 1;
    for (; depth < maxDepth; ++depth) {
        auto leftValue = _candidats.begin();
        while (leftValue != _candidats.end()) {
            auto rightValues = _candidats.begin();
            while (rightValues != _candidats.end()) {
                if (rightValues == leftValue) {
                    ++rightValues;
                    continue;
                }
                auto pick = rightValues;
                vector<Evaluation> testVals;
                testVals.push_back(*leftValue);
                int d = depth;
                while (d > 0 && pick != _candidats.end()) {
                    testVals.push_back(*pick);
                    --d;
                    ++pick;
                }
                Evaluation combined = e.evaluate(testVals);
                if (isSignificant(combined, testVals)) {
                    result.push_back(combined);
                    picked.insert(picked.end(), testVals.begin(), testVals.end());
                }
                ++rightValues;
            }
            ++leftValue;
        }
    }
    sort(picked.begin(), picked.end());
    auto lastUnique = unique(picked.begin(), picked.end());
    deque<Evaluation> uniqPicked;
    uniqPicked.insert(uniqPicked.end(), picked.begin(), lastUnique);
    for_each(_candidats.begin(), _candidats.end(), [&uniqPicked, &result](Evaluation e) {
        if (uniqPicked.end() != find_if(uniqPicked.begin(), uniqPicked.end(), [e](const Evaluation &p){
            return (p.mostProbable().samplers() == e.mostProbable().samplers());
        })) {
            result.push_back(e);
        }
    });
    _resulting = result;
    return static_cast<unsigned>(depth);
}

deque<Evaluation> EvaluationCombiner::result() const
{
    return _resulting;
}

bool EvaluationCombiner::isSignificant(const models::Evaluation &combined, const std::vector<models::Evaluation> &sources) const
{
    return true;
}

} // namespace services
} // namespace mimir
