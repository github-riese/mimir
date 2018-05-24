#ifndef SAMPLE_H
#define SAMPLE_H

#include <cstdlib>

namespace mimir {
namespace models {

class Sample
{
public:
    explicit Sample(size_t classifier, size_t value, unsigned count = 1);
    size_t classifier() const;

    size_t value() const;

    unsigned count() const;

private:
    size_t _classifier;
    size_t _value;
    unsigned _count;
};

} // namespace models
} // namespace mimir

#endif // SAMPLE_H
