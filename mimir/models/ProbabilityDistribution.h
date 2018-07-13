#ifndef PROBABILITYDISTRIBUTION_H
#define PROBABILITYDISTRIBUTION_H

#include <map>
#include <utility>
#include <ostream>

#include "KeyValuePair.h"
#include "Probability.h"
#include "ValueIndex.h"
#include "../services/NameResolver.h"

namespace mimir {
namespace models {

class ProbabilityDistribution
{
public:
    ProbabilityDistribution(std::vector<std::pair<ValueIndex, Probability>>);
    ProbabilityDistribution(std::vector<Probability>, std::vector<ValueIndex> = std::vector<ValueIndex>());
    ProbabilityDistribution(std::map<ValueIndex, Probability>);
    Probability vectorLength() const;
    Probability argMax() const;
    ValueIndex mostProbable() const;
    Probability argMin() const;
    ValueIndex leastProbable() const;
    std::pair<ValueIndex, Probability> max() const;
    std::pair<ValueIndex, Probability> min() const;
    bool operator <(const ProbabilityDistribution &rhs) const;
    std::vector<Probability> probabilities() const;
    std::vector<ValueIndex> classifiers() const;
    std::vector<NamedProbability> distribution() const;

    Probability probabilityOf(ValueIndex) const;
    std::ostream &dump(std::ostream &, services::NameResolver &) const;
private:
    void analyze();
private:
    std::vector<ValueIndex> _classifiers;
    std::vector<Probability> _probabilities;
    long double _vectorLength = .0l;
    Probability _maxProb;
    Probability _minProb;
    ValueIndex _maxVal;
    ValueIndex _minVal;
};

} // namespace models
} // namespace mimir

#endif // PROBABILITYDISTRIBUTION_H
