#ifndef HELPERS_H
#define HELPERS_H

#include <deque>
#include <vector>

#include <numeric>

#include "../models/Evaluation.h"
#include "../models/Probability.h"

namespace mimir {
namespace helpers {

template <typename InputIterator>
inline long double variance(InputIterator first, InputIterator last)
{
    auto distance = std::distance(first, last);
    if (distance < 2) {
        return 0.l;
    }
    struct VarianceData {
        long double sums = 0; long double squares = 0;
    };
    auto v = std::accumulate(first, last, VarianceData(),
                            [](VarianceData const &left, mimir::models::Probability const&right) -> VarianceData {
                                long double v = right.value();
                                return VarianceData{left.sums + v, left.squares + v*v };
                            }
            );
    return (v.squares - (v.sums * v.sums) / distance) / (distance - 1);
}

template <typename InputIterator>
inline long double deviation(InputIterator first, InputIterator last)
{
    return std::sqrtl(variance(first, last));
}

long double mean(const std::deque<mimir::models::Probability> &);
long double mean(const std::vector<mimir::models::Probability>&);

template<typename InputIterator>
long double mean(InputIterator first, InputIterator last)
{
    auto distance = std::distance(first, last);
    auto sum = std::accumulate(first, last, mimir::models::Probability(0));
    return sum.value() / distance;
}

long double deviation(const std::deque<mimir::models::Probability> &);
long double deviation(const std::vector<mimir::models::Probability> &);

std::vector<mimir::models::Probability> fetchMostProbables(std::vector<mimir::models::Evaluation> const &input);
} // namespace helpers
} // namespace mimir

#endif // HELPERS_H
