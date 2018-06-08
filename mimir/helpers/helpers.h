#ifndef HELPERS_H
#define HELPERS_H

#include <deque>
#include <vector>

#include "../models/Evaluation.h"
#include "../models/Probability.h"

namespace mimir {
namespace helpers {

long double mean(const std::deque<mimir::models::Probability> &);
long double mean(const std::vector<mimir::models::Probability>&);

long double deviation(const std::deque<mimir::models::Probability> &);
long double deviation(const std::vector<mimir::models::Probability> &);

} // namespace helpers
} // namespace mimir

#endif // HELPERS_H
