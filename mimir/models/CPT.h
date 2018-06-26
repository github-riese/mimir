#ifndef CPT_H
#define CPT_H

#include <iostream>
#include <utility>
#include <vector>

#include "../services/NameResolver.h"
#include "Probability.h"
#include "ValueIndex.h"

namespace mimir {
namespace models {

/**
 * @brief Conditional Probability table
 */
class CPT
{
public:
    CPT(std::vector<std::vector<ValueIndex>>);
    Probability probability(std::vector<ValueIndex> const &);
    std::ostream &dump(std::ostream&, services::NameResolver &) const;
private:
    void calculateProbabilities(std::vector<std::vector<ValueIndex>>);
private:
    std::vector<std::pair<std::vector<ValueIndex>, Probability>> _proabilities;
};

} // namespace models
} // namespace mimir

#endif // CPT_H
