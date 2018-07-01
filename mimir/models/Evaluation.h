#ifndef EVALUATION_H
#define EVALUATION_H

#include <map>
#include <vector>

#include "Probability.h"
#include "ProbabilityWithPriors.h"
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
    Evaluation(std::vector<std::vector<ValueIndex>> sources);

    void addProbability(ValueIndex classification, const ProbabilityWithPriors &probability);
    void addProbability(ValueIndex classification, const Probability &probability, const Probability &likelyhood, const Probability &classProbability);

    void evaluate();

    inline void setEvidence(const Probability &p) { _evidence = p; }

    inline Probability evidence() const { return _evidence; }
    inline ValueIndex mostProbableClass() const { return _mostProbable; }

    inline Probability mostProbable() const
    {
        if (_mostProbable == ValueIndex::NoIndex) return Probability();
        return _probabilities.at(_mostProbable).p;
    }

    inline ProbabilityWithPriors mostProbableEx()
    {
        if (_mostProbable == ValueIndex::NoIndex) return ProbabilityWithPriors();
        auto p = _probabilities.at(_mostProbable);
        return { p.p, p.likelyhood, p.classP, _evidence };
    }

    inline Probability probabilityByClassification(mimir::models::ValueIndex i) const
    {
        auto p = _probabilities.find(i);
        if (p == _probabilities.end()) return Probability();
        return (*p).second.p;
    }

    inline ProbabilityWithPriors probabilityByClassificationEx(mimir::models::ValueIndex i) const
    {
        auto p = _probabilities.find(i);
        if (p == _probabilities.end()) return ProbabilityWithPriors();
        return { (*p).second.p, (*p).second.likelyhood, (*p).second.classP, _evidence };
    }

    inline long double mean() const { return _mean; }
    inline long double deviation() const { return _deviation; }
    std::vector<mimir::models::ValueIndex> classifications() const;

    bool operator<(const Evaluation &rhs) const;
    bool operator==(const Evaluation &rhs) const;

    inline std::vector<std::vector<ValueIndex>> const & sources() const
    {
        return _sources;
    }

private:
    std::vector<std::vector<ValueIndex>> _sources;
    std::map<ValueIndex, ProbabilityEx> _probabilities;
    Probability _evidence = Probability();
    ValueIndex _mostProbable = ValueIndex();
    long double _mean;
    long double _deviation;
    bool _dirty = true;
};

} // namespace models
} // namespace mimir

#endif // EVALUATION_H
