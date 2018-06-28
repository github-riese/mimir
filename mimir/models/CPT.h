#ifndef CPT_H
#define CPT_H

#include <iostream>
#include <utility>
#include <vector>

#include "Probability.h"
#include "ProbabilityDistribution.h"
#include "ValueIndex.h"

#include "../services/NameResolver.h"

namespace mimir {
namespace models {

/**
 * @brief Conditional Probability table
 */
class CPT
{
public:
    CPT(std::vector<ValueIndex>, std::vector<std::vector<ValueIndex>>);
    Probability probability(std::vector<ValueIndex> columns, std::vector<ValueIndex> values) const;
    Probability evidence(std::vector<ValueIndex> const &);
    ProbabilityDistribution classify(const std::vector<ValueIndex> &columns, std::vector<ValueIndex> const &, ValueIndex);
    std::vector<ValueIndex> distinctValues(ValueIndex field) const;
    std::ostream &dump(std::ostream&, services::NameResolver &) const;
private:
    void calculateProbabilities(std::vector<std::vector<ValueIndex>>);
    long int fieldIndex(ValueIndex name) const;
    std::vector<std::pair<long, ValueIndex> > buildMatchRule(std::vector<ValueIndex>, std::vector<ValueIndex>) const;
private:
    std::vector<ValueIndex> _fields;
    std::vector<std::pair<std::vector<ValueIndex>, Probability>> _proabilities;
};

} // namespace models
} // namespace mimir

#endif // CPT_H
