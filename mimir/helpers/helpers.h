#ifndef HELPERS_H
#define HELPERS_H

#include <deque>
#include <valarray>
#include <vector>

#include <numeric>

#include "../models/Probability.h"

namespace mimir {
namespace helpers {

template <typename InputIterator>
inline double variance(InputIterator first, InputIterator last)
{
    auto distance = std::distance(first, last);
    if (distance < 2) {
        return 0.l;
    }
    struct VarianceData {
        double sums = 0;
        double sumOfSquares = 0;
    };
    auto v = std::accumulate(first, last, VarianceData(),
                            [](VarianceData const &left, mimir::models::Probability const&right) -> VarianceData {
                                double v = right.value();
                                return VarianceData{left.sums + v, left.sumOfSquares + v*v };
                            }
            );
    return (v.sumOfSquares - (v.sums * v.sums) / distance) / (distance - 1);
}

template <typename Container>
inline double variance(Container const &container)
{
    return variance(std::begin(container), std::end(container));
}

template <typename InputIterator>
inline double deviation(InputIterator first, InputIterator last)
{
    return std::sqrt(variance(first, last));
}

double mean(const std::deque<mimir::models::Probability> &);
double mean(const std::vector<mimir::models::Probability>&);

template<typename InputIterator>
double mean(InputIterator first, InputIterator last)
{
    auto distance = std::distance(first, last);
    auto sum = std::accumulate(first, last, mimir::models::Probability(0));
    return sum.value() / distance;
}

double deviation(const std::deque<mimir::models::Probability> &);
double deviation(const std::vector<mimir::models::Probability> &);

std::vector<double> toVector(std::valarray<double> const&);
std::valarray<double> toArray(std::vector<double> const &);

struct PackDouble
{
    uint64_t operator()(double value);
};

struct UnpackDouble
{
    double operator()(uint64_t packed);
};

} // namespace helpers
} // namespace mimir

#endif // HELPERS_H
