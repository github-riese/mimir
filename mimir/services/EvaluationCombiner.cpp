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


} // namespace services
} // namespace mimir
