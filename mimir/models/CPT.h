#ifndef CPT_H
#define CPT_H

#include <iostream>
#include <utility>
#include <vector>

#include "KeyValuePair.h"
#include "Probability.h"
#include "ProbabilityDistribution.h"
#include "ValueIndex.h"

namespace mimir {
namespace models {

/**
 * @brief Conditional Probability table
 */
class CPT
{
public:
    struct Row
    {
        std::vector<ValueIndex> values;
        Probability probability;
        bool matchesInput(std::vector<ColumnIndexValuePair> const&) const;
    };
public:
    CPT(std::vector<ValueIndex>, std::vector<std::vector<ValueIndex>>);
    std::vector<ValueIndex> fieldNames() const;
    Probability probability(std::vector<ColumnNameValuePair> values) const;
    Probability probability(std::vector<ColumnIndexValuePair> values) const;

    ProbabilityDistribution classify(ValueIndex, std::vector<ColumnNameValuePair> const &);
    ProbabilityDistribution classify(long, std::vector<ColumnIndexValuePair> const&);
    std::vector<ValueIndex> distinctValues(ValueIndex field) const;
    std::vector<ValueIndex> distinctValues(long fieldIndex) const;
    std::ostream &dump(std::ostream&, services::NameResolver &) const;
    long int fieldIndex(ValueIndex name) const;
    ValueIndex fieldName(long int) const;

private:
    void calculateProbabilities(std::vector<std::vector<ValueIndex>>);
    std::vector<ColumnIndexValuePair> buildMatchRule(std::vector<ColumnNameValuePair> const&values) const;
private:
    std::vector<ValueIndex> _fieldNames;
    std::vector<Row> _proabilities;
};

} // namespace models
} // namespace mimir

#endif // CPT_H
