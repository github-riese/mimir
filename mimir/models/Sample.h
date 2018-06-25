#ifndef SAMPLE_H
#define SAMPLE_H

#include <utility>
#include <vector>

#include "ValueIndex.h"
#include "ValueCounter.h"

namespace mimir {
namespace models {

class Sample
{
public:
    Sample(ValueIndex classifier, std::vector<ValueCounter> values);
    Sample(ValueIndex classifier, std::vector<ValueIndex> values);
    Sample(ValueIndex classifer, ValueIndex, unsigned long);
    ValueIndex classifier() const;

    std::vector<ValueCounter> values() const;

    unsigned long count() const;

private:
    ValueIndex _classifier;
    std::vector<ValueCounter> _values;
    unsigned long _count;
};

} // namespace models
} // namespace mimir

#endif // SAMPLE_H
