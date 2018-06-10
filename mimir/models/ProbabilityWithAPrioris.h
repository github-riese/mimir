#ifndef PROBABILITY_WITH_APRIORIS
#define PROBABILITY_WITH_APRIORIS

#include "Probability.h"

namespace mimir {
namespace models {

class ProbabilityWithAPrioris
{
public:
    inline explicit ProbabilityWithAPrioris() {}
    inline ProbabilityWithAPrioris(Probability probability, Probability aPrioriA, Probability aPrioriB) :
        _probability(probability),
        _aPrioriA(aPrioriA),
        _aPrioriB(aPrioriB)
    {}
    inline operator Probability() { return  _probability; }
    inline Probability probability() const { return _probability; }
    inline Probability aPrioryA() const { return _aPrioriA; }
    inline Probability aPrioryB() const { return _aPrioriB; }
private:
    Probability _probability;
    Probability _aPrioriA;
    Probability _aPrioriB;
};

}
}
#endif // PROBABILITY_WITH_APRIORIS
