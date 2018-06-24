#ifndef PROBABILITY_WITH_PRIORS
#define PROBABILITY_WITH_PRIORS

#include "Probability.h"

namespace mimir {
namespace models {

class ProbabilityWithPriors
{
public:
    inline explicit ProbabilityWithPriors() {}
    inline ProbabilityWithPriors(Probability probability, Probability likelyhood, Probability prior, Probability evidence) :
        _probability(probability),
        _likelihood(likelyhood),
        _prior(prior),
        _evidence(evidence)
    {}
    inline operator Probability() { return  _probability; }
    inline Probability probability() const { return _probability; }
    inline Probability prior() const { return _prior; }
    inline Probability evidence() const { return _evidence; }
    inline Probability likelihood() const { return _likelihood; }
private:
    Probability _probability;
    Probability _likelihood;
    Probability _prior;
    Probability _evidence;
};

}
}
#endif // PROBABILITY_WITH_PRIORS
