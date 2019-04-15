#ifndef NAMEDPROBABILITY_H
#define NAMEDPROBABILITY_H

#include <vector>
#include <tuple>
#include "models/Probability.h"
#include "models/ValueIndex.h"

namespace mimir {
namespace models {

using NamedProbability = std::pair<ValueIndex, Probability>;
using NamedProbabilityVector = std::vector<NamedProbability>;

}
}
#endif // NAMEDPROBABILITY_H
