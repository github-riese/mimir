#ifndef BAYESCALCULATOR_H
#define BAYESCALCULATOR_H

#include "../models/Probability.h"

namespace mimir {
namespace services {

struct BayesCalculator
{
    inline static long double calculate(long double aPrioryBinA, long double aPrioriA, long double aPrioriB)
    {
        return (aPrioryBinA * aPrioriA)/aPrioriB;
    }

    inline static long double calculate(unsigned long countInClassAndValue, unsigned long totalInClass, unsigned long totalInValue, unsigned long totalSamples)
    {
        return  calculate(static_cast<long double>(countInClassAndValue) / static_cast<long double>(totalInClass),
                    (static_cast<long double>(totalInClass) / static_cast<long double>(totalSamples)),
                (static_cast<long double>(totalInValue) / static_cast<long double>(totalSamples)));
    }
};

} // namespace services
} // namespace mimir

#endif // BAYESCALCULATOR_H
