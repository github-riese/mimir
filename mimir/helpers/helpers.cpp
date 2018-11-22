#include "helpers.h"

#include <numeric>

using std::fabsl;

using std::deque;
using std::vector;

using mimir::models::Probability;

namespace mimir {
namespace helpers {

long double mean(const std::deque<models::Probability> &probabilities)
{
    return mean(probabilities.begin(), probabilities.end());
}

long double mean(const std::vector<models::Probability> &probabilities)
{
    return mean(probabilities.begin(), probabilities.end());
}

long double deviation(const std::deque<models::Probability> &probabilities)
{
    return deviation(probabilities.begin(), probabilities.end());
}

long double deviation(const std::vector<models::Probability> &probabilities)
{
    return deviation(probabilities.begin(), probabilities.end());
}

} // namespace helpers
} // namespace mimir
