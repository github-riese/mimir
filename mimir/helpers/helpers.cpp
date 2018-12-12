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

std::vector<double> toVector(std::valarray<double> const&array)
{
    return {std::begin(array), std::end(array)};
}

std::valarray<double> toArray(std::vector<double> const &vector)
{
    std::valarray<double> t(vector.size());
    std::copy(std::begin(vector), std::end(vector), std::begin(t));
    return t;
}

} // namespace helpers
} // namespace mimir
