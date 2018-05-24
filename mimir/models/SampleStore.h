#ifndef SAMPLESTORE_H
#define SAMPLESTORE_H

#include <map>
#include <vector>

#include "ValueCounter.h"
#include "Sample.h"

using std::map;
using std::vector;
using std::pair;

namespace mimir {
namespace models {

class SampleStore : public map<size_t, vector<ValueCounter>>
{
public:
    SampleStore();
    void add(const Sample &);
    unsigned total() const;
    unsigned countInValue(size_t value) const;
    unsigned countInClass(size_t classifier) const;
    unsigned count(size_t classifier, size_t value) const;
};

} // namespace models
} // namespace mimir

#endif // SAMPLESTORE_H
