#ifndef SAMPLE_H
#define SAMPLE_H

#include <cstdlib>

#include "ValueIndex.h"

namespace mimir {
namespace models {

class Sample
{
public:
    explicit Sample(ValueIndex classifier, ValueIndex valueIndex, unsigned long count = 1);
    ValueIndex classifier() const;

    ValueIndex valueIndex() const;

    unsigned long count() const;

private:
    ValueIndex _classifier;
    ValueIndex _valueIndex;
    unsigned _count;
};

} // namespace models
} // namespace mimir

#endif // SAMPLE_H
