#include "helpers.h"

#include <numeric>

using std::fabsl;

using std::deque;
using std::vector;

using mimir::models::Evaluation;
using mimir::models::Probability;

namespace mimir {
namespace helpers {

template <typename Iterator>
inline long double mean(Iterator first, Iterator last)
{
    long double result = 0;
    auto tmp = first;
    while (first != last) {
        result += (*(first++)).probability();
    }
    return result / static_cast<long double>(last - tmp);
}

template <typename Iterator>
inline long double deviation(long double mean, Iterator first, Iterator last)
{
    long double deviation = 0;
    auto tmp = first;
    while (first != last) {
        deviation += fabsl((*(first++)).probability()-mean);
    }
    return deviation / static_cast<long double>(last - tmp);
}

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
    return deviation(mean(probabilities), probabilities.begin(), probabilities.end());
}

long double deviation(const std::vector<models::Probability> &probabilities)
{
    return deviation(mean(probabilities), probabilities.begin(), probabilities.end());
}


} // namespace helpers
} // namespace mimir
