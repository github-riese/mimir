#ifndef PROBABILITY_H
#define PROBABILITY_H

namespace mimir {
namespace models {

class Probability
{
public:
    explicit Probability() : _probability(1) {}
    inline explicit Probability(long double probability, long double cardinality) : _probability(probability), _cardinality(cardinality) {}
    Probability(const Probability&) = default;
    Probability(Probability &&) = default;
    Probability &operator=(const Probability&) = default;

    inline bool operator <(const Probability &rhs) { return _probability < rhs._probability; }

    inline long double probability() const { return _probability; }
    inline void setProbability(long double probability) { _probability = probability; }

    inline long double cardinality() const { return _cardinality; }
    inline void setCardinality(long double cardinality) { _cardinality = cardinality; }

private:
    long double _probability;
    long double _cardinality;
};

} // namespace models
} // namespace mimir
inline mimir::models::Probability operator "" _p (long double p) { return mimir::models::Probability(p, 1); }

#endif // PROBABILITY_H
