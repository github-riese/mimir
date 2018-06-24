#include "helpers.h"

#include <numeric>

using std::fabsl;

using std::deque;
using std::vector;

using mimir::models::Evaluation;
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

std::vector<mimir::models::Probability> fetchMostProbables(std::vector<mimir::models::Evaluation> const &input)
{
    std::vector<mimir::models::Probability> mostProbables;
    std::for_each(input.begin(), input.end(), [&mostProbables](mimir::models::Evaluation const& item){
        mostProbables.push_back(item.mostProbable());
    });
    return mostProbables;
}

} // namespace helpers
} // namespace mimir
