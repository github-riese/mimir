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
inline float variance(InputIterator first, InputIterator last)
{
    auto distance = std::distance(first, last);
    if (distance < 2) {
        return 0.l;
    }
    struct VarianceData {
        float sums = 0;
        float sumOfSquares = 0;
    };
    auto v = std::accumulate(first, last, VarianceData(),
                            [](VarianceData const &left, mimir::models::Probability const&right) -> VarianceData {
                                float v = right.value();
                                return VarianceData{left.sums + v, left.sumOfSquares + v*v };
                            }
            );
    return (v.sumOfSquares - (v.sums * v.sums) / distance) / (distance - 1);
}

template <typename Container>
inline float variance(Container const &container)
{
    return variance(std::begin(container), std::end(container));
}

template <typename InputIterator>
inline float deviation(InputIterator first, InputIterator last)
{
    return std::sqrt(variance(first, last));
}

float mean(const std::deque<mimir::models::Probability> &);
float mean(const std::vector<mimir::models::Probability>&);

template<typename InputIterator>
float mean(InputIterator first, InputIterator last)
{
    auto distance = std::distance(first, last);
    auto sum = std::accumulate(first, last, mimir::models::Probability(0));
    return sum.value() / distance;
}

float deviation(const std::deque<mimir::models::Probability> &);
float deviation(const std::vector<mimir::models::Probability> &);

std::vector<float> toVector(std::valarray<float> const&);
std::valarray<float> toArray(std::vector<float> const &);

struct PackDouble
{
    uint64_t operator()(float value);
};

struct UnpackDouble
{
    float operator()(uint64_t packed);
};

template <typename ContainerType>
bool containerHas(ContainerType const &container, typename ContainerType::value_type const &value) noexcept
{
    return std::find(container.cbegin(), container.cend(), value) != container.end();
}

} // namespace helpers
} // namespace mimir

#endif // HELPERS_H
