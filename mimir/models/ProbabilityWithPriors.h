#ifndef PROBABILITY_WITH_PRIORS
#define PROBABILITY_WITH_PRIORS

#include "Probability.h"

namespace mimir {
namespace models {

class ProbabilityWithPriors
{
public:
    inline explicit ProbabilityWithPriors() {}
    inline ProbabilityWithPriors(Probability probability, Probability likelyhood, Probability classProbability, Probability valueProbability) :
        _probability(probability),
        _likelyHood(likelyhood),
        _classProbability(classProbability),
        _valueProbability(valueProbability)
    {}
    inline operator Probability() { return  _probability; }
    inline Probability probability() const { return _probability; }
    inline Probability classProbability() const { return _classProbability; }
    inline Probability valueProbability() const { return _valueProbability; }
    inline Probability likelyhood() const { return _likelyHood; }
private:
    Probability _probability;
    Probability _likelyHood;
    Probability _classProbability;
    Probability _valueProbability;
};

}
}
#endif // PROBABILITY_WITH_PRIORS
