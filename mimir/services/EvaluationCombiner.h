#ifndef EVALUATIONCOMBINER_H
#define EVALUATIONCOMBINER_H

#include <deque>
#include <tuple>
#include <vector>

#include "../models/Evaluation.h"
#include "DataStore.h"

namespace mimir {
namespace services {

class EvaluationCombiner
{
public:
    EvaluationCombiner(DataStore &dataStore);
    void addEvaluations(std::deque<mimir::models::Evaluation>);
    unsigned findClusters(int maxDepth = -1);
    std::deque<mimir::models::Evaluation> result() const;
private:
    std::pair<Sampler, Sampler> buildNextSubSampler();
    bool isSignificant(const mimir::models::Evaluation &combined, const std::vector<mimir::models::Evaluation> &sources) const;
private:
    DataStore &_dataStore;
    std::deque<mimir::models::Evaluation> _candidats;
    std::deque<mimir::models::Evaluation> _resulting;
    std::vector<mimir::models::ValueIndex>::iterator _currentClassBuildIterator;
    std::vector<mimir::models::ValueIndex>::iterator _currentValueBuildIterator;
};

} // namespace services
} // namespace mimir

#endif // EVALUATIONCOMBINER_H
