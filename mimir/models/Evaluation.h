#ifndef EVALUATION_H
#define EVALUATION_H

#include <map>
#include <tuple>

#include "Probability.h"
#include "ValueIndex.h"

namespace mimir {
namespace models {

class Evaluation
{
public:
    Evaluation();

    void addProbability(ValueIndex classification, const Probability &probability);

    inline ValueIndex mostProbableClass() const { return _mostProbable; }

    inline Probability mostProbable() const
    {
        if (_mostProbable == ValueIndex::NoIndex) return Probability();
        return _probabilities.at(_mostProbable);
    }

    inline Probability probabilityByClassification(mimir::models::ValueIndex i) const
    {
        auto p = _probabilities.find(i);
        if (p == _probabilities.end()) return Probability();
        return (*p).second;
    }

    inline long double mean() const { return _mean; }
    inline long double deviation() const { return _deviation; }
    std::vector<mimir::models::ValueIndex> classifications() const;

    bool operator<(const Evaluation &rhs) const;
    bool operator==(const Evaluation &rhs) const;

private:
    std::map<ValueIndex, Probability> _probabilities;
    ValueIndex _mostProbable = ValueIndex();
    long double _mean;
    long double _deviation;
};

} // namespace models
} // namespace mimir

#endif // EVALUATION_H
