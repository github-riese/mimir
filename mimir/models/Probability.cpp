#include "Probability.h"

namespace mimir {
namespace models {

Probability::Probability(long double probability, long double significance, long double cardinality) :
    _probability(probability),
    _significance(significance),
    _cardinality(cardinality)
{
}

bool Probability::operator <(const Probability &rhs)
{
    return _probability < rhs._probability;
}

long double Probability::probability() const
{
    return _probability;
}

void Probability::setProbability(long double probability)
{
    _probability = probability;
}

long double Probability::significance() const
{
    return _significance;
}

void Probability::setSignificance(long double significance)
{
    _significance = significance;
}

long double Probability::cardinality() const
{
    return _cardinality;
}

void Probability::setCardinality(long double cardinality)
{
    _cardinality = cardinality;
}

} // namespace models
} // namespace mimir
