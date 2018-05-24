#ifndef PROBABILITY_H
#define PROBABILITY_H

namespace mimir {
namespace models {

class Probability
{
public:
    explicit Probability(long double probability, long double significance, long double cardinality);
    Probability(const Probability&) = default;
    Probability(Probability &&) = default;
    Probability &operator=(const Probability&) = default;

    bool operator <(const Probability &rhs);

    long double probability() const;
    void setProbability(long double probability);

    long double significance() const;
    void setSignificance(long double significance);

    long double cardinality() const;
    void setCardinality(long double cardinality);

private:
    long double _probability;
    long double _significance;
    long double _cardinality;
};

} // namespace models
} // namespace mimir

#endif // PROBABILITY_H
