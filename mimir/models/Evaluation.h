#ifndef EVALUATION_H
#define EVALUATION_H

#include <map>
#include <vector>

#include "Probability.h"
#include "ProbabilityWithAPrioris.h"
#include "ValueIndex.h"

namespace mimir {
namespace models {

class Evaluation
{
    struct ProbabilityEx {
        Probability p;
        Probability likelyhood;
        Probability classP;
        bool operator ==(const ProbabilityEx &) const;
    };
public:
    Evaluation(std::vector<std::vector<ValueIndex>> samplers);

    void addProbability(ValueIndex classification, const ProbabilityWithAPrioris &probability);
    void addProbability(ValueIndex classification, const Probability &probability, const Probability &likelyhood, const Probability &classProbability);

    inline Probability valueProbability() const { return _valueProbability; }
    inline ValueIndex mostProbableClass() const { return _mostProbable; }

    inline Probability mostProbable() const
    {
        if (_mostProbable == ValueIndex::NoIndex) return Probability();
        return _probabilities.at(_mostProbable).p;
    }

    inline ProbabilityWithAPrioris mostProbableEx()
    {
        if (_mostProbable == ValueIndex::NoIndex) return ProbabilityWithAPrioris();
        auto p = _probabilities.at(_mostProbable);
        return { p.p, p.likelyhood, p.classP, _valueProbability };
    }

    inline Probability probabilityByClassification(mimir::models::ValueIndex i) const
    {
        auto p = _probabilities.find(i);
        if (p == _probabilities.end()) return Probability();
        return (*p).second.p;
    }

    inline ProbabilityWithAPrioris probabilityByClassificationEx(mimir::models::ValueIndex i) const
    {
        auto p = _probabilities.find(i);
        if (p == _probabilities.end()) return ProbabilityWithAPrioris();
        return { (*p).second.p, (*p).second.likelyhood, (*p).second.classP, _valueProbability };
    }

    inline long double mean() const { return _mean; }
    inline long double deviation() const { return _deviation; }
    std::vector<mimir::models::ValueIndex> classifications() const;

    bool operator<(const Evaluation &rhs) const;
    bool operator==(const Evaluation &rhs) const;

    inline std::vector<std::vector<ValueIndex>> const & samplers() const
    {
        return _samplers;
    }

private:
    void evaluate();
private:
    std::vector<std::vector<ValueIndex>> _samplers;
    std::map<ValueIndex, ProbabilityEx> _probabilities;
    Probability _valueProbability;
    ValueIndex _mostProbable = ValueIndex();
    long double _mean;
    long double _deviation;
};

} // namespace models
} // namespace mimir

#endif // EVALUATION_H
