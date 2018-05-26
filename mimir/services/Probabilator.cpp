#include "Probabilator.h"

#include <numeric>

using std::accumulate;

using std::string;
using std::vector;

using mimir::models::InvalidIndex;
using mimir::models::Probability;
using mimir::models::Sample;
using mimir::models::ValueIndex;

namespace mimir {
namespace services {

Probabilator::Probabilator(const string &name) :
    _name(name)
{
}

void Probabilator::addSampler(const Sampler &sampler)
{
    _samplers.push_back(sampler);
}

Probability Probabilator::evaluate(ValueIndex classification, ValueIndex value) const
{
    /*if (_samplers.size() == 1) {
        return calculate(_samplers.front(), classification, value);
    }*/
    vector<Probability> results;
    unsigned long totalInClass = 0;
    unsigned long total = 0;
    for (auto sampler : _samplers) {
        if (!sampler.total() || !sampler.countInClass(classification)) {
            continue;
        }
        results.push_back(calculate(sampler, classification, value));
        totalInClass += sampler.countInClass(classification);
        total += sampler.total();
    }
    long double pClassVal_n = 1.L;
    long double pVal_n = 1.L;
    for (auto result : results) {
        pClassVal_n *= result.probability();
        pVal_n *= result.cardinality();
    }
    long double totalCardinality = ((long double)totalInClass/(long double)total);
    return Probability((pClassVal_n * totalCardinality)/pVal_n, pVal_n);
}

Probability Probabilator::calculate(const Sampler &sampler, ValueIndex classification, ValueIndex value) const
{
    // P(class|value) = (P(value|class) * P(value))/(P(class))
    // P(value|class) = count of class in value / count in value
    // P(value) = count in value / count in total
    // P(class) = count in class / count in total
    long double countInClassInValue = (long double) sampler.count(classification, value);
    long double countInValue = (long double) sampler.countInValue(value);
    long double countInClass = (long double) sampler.countInClass(classification);
    long double total = (long double) sampler.total();
    long double pBA = countInClassInValue / countInClass;
    long double pA = countInClass / total;
    long double pB = countInValue / total;
    long double pAB = (pBA * pA)/pB; // that's Bayes for you.
    return Probability{pAB, pA};
}

} // namespace services
} // namespace mimir
