#ifndef EVALUATION_H
#define EVALUATION_H

#include <map>

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
        if (_mostProbable == InvalidIndex()) return Probability();
        return _probabilities.at(_mostProbable);
    }

    inline Probability probabilityByClassification(mimir::models::ValueIndex i) const
    {
        auto p = _probabilities.find(i);
        if (p == _probabilities.end()) return Probability();
        return (*p).second;
    }

    std::vector<mimir::models::ValueIndex> probableClassifications() const;

    inline long double average() const { return _average; }

private:
    std::map<ValueIndex, Probability> _probabilities;
    ValueIndex _mostProbable = InvalidIndex();
    long double _average = .5L;
};

} // namespace models
} // namespace mimir

#endif // EVALUATION_H
