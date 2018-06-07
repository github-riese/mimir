#ifndef EVALUATIONCOMBINER_H
#define EVALUATIONCOMBINER_H

#include <deque>

#include "../models/Evaluation.h"

namespace mimir {
namespace services {

class EvaluationCombiner
{
public:
    EvaluationCombiner();
    void addEvaluations(std::deque<mimir::models::Evaluation>);
    unsigned findClusters(unsigned maxDepth = 3);
    std::deque<mimir::models::Evaluation> result() const;
private:
    std::deque<mimir::models::Evaluation> _candidats;
    std::deque<mimir::models::Evaluation> _resulting;
};

} // namespace services
} // namespace mimir

#endif // EVALUATIONCOMBINER_H
