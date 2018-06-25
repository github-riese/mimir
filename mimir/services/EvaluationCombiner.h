#ifndef EVALUATIONCOMBINER_H
#define EVALUATIONCOMBINER_H

#include <deque>
#include <vector>

#include "../models/ValueIndex.h"
#include "DataStore.h"

namespace mimir {
namespace services {

class EvaluationCombiner
{
public:
    EvaluationCombiner(DataStore &dataStore);
private:
    DataStore &_dataStore;
};

} // namespace services
} // namespace mimir

#endif // EVALUATIONCOMBINER_H
