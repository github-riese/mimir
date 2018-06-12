#ifndef PROBABILITY_WITH_APRIORIS
#define PROBABILITY_WITH_APRIORIS

#include "Probability.h"

namespace mimir {
namespace models {

class ProbabilityWithAPrioris
{
public:
    inline explicit ProbabilityWithAPrioris() {}
    inline ProbabilityWithAPrioris(Probability probability, Probability likelyhood, Probability probOfClass, Probability probOfValue) :
        _probability(probability),
        _likelyHood(likelyhood),
        _probOfClass(probOfClass),
        _probOfValue(probOfValue)
    {}
    inline operator Probability() { return  _probability; }
    inline Probability probability() const { return _probability; }
    inline Probability probOfClass() const { return _probOfClass; }
    inline Probability probOfValue() const { return _probOfValue; }
    inline Probability likelyHood() const { return _likelyHood; }
private:
    Probability _probability;
    Probability _likelyHood;
    Probability _probOfClass;
    Probability _probOfValue;
};

}
}
#endif // PROBABILITY_WITH_APRIORIS
