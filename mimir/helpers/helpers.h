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
long double deviation(InputIterator first, InputIterator last)
{
    auto distance = std::distance(first, last);
    struct DevVals {
        long double sums = 0; long double squares = 0;
    };
    auto v = std::accumulate(first, last, DevVals(), [](DevVals const &left, mimir::models::Probability const&right) ->DevVals{
                long double v = right.value();
                return DevVals{left.sums + v, left.squares + v*v };
    });
    return std::sqrtl((v.squares - (v.sums * v.sums) / distance) / (distance - 1));
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
