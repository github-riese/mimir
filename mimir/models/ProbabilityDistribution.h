#ifndef PROBABILITYDISTRIBUTION_H
#define PROBABILITYDISTRIBUTION_H

#include <map>
#include <utility>
#include <ostream>

#include "KeyValuePair.h"
#include "NamedProbability.h"
#include "Probability.h"
#include "ValueIndex.h"
#include "../services/NameResolver.h"

namespace mimir {
namespace models {

class ProbabilityDistribution
{
public:
    ProbabilityDistribution() = default;
    ProbabilityDistribution(std::vector<std::pair<ValueIndex, Probability>>);
    ProbabilityDistribution(std::vector<Probability>, std::vector<ValueIndex> = std::vector<ValueIndex>());
    ProbabilityDistribution(std::map<ValueIndex, Probability>);
    ProbabilityDistribution(std::map<std::vector<ValueIndex>, Probability>);
    double vectorLength() const;
    Probability argMin() const;
    Probability argMax() const;
    ValueIndex mostProbable() const;
    ValueIndex leastProbable() const;
    ValueProbability max() const;
    ValueProbability min() const;
    bool operator <(const ProbabilityDistribution &rhs) const;
    std::vector<Probability> probabilities() const;
    std::vector<ValueIndex> classifiers() const;
    ValueProbabilityVector distribution() const;

    Probability probabilityOf(ValueIndex) const;
    std::ostream &dump(std::ostream &, services::NameResolver &) const;
private:
    void analyze();
private:
    std::vector<ValueIndex> _classifiers;
    std::vector<Probability> _probabilities;
    double _vectorLength = .0l;
    Probability _maxProb;
    Probability _minProb;
    ValueIndex _maxVal;
    ValueIndex _minVal;
};

} // namespace models
} // namespace mimir

#endif // PROBABILITYDISTRIBUTION_H
