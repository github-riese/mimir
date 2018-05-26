#ifndef SAMPLESTORE_H
#define SAMPLESTORE_H

#include <map>
#include <vector>

#include "Sample.h"
#include "ValueCounter.h"
#include "ValueIndex.h"

using std::map;
using std::vector;
using std::pair;

namespace mimir {
namespace models {

class SampleStore : public map<ValueIndex, vector<ValueCounter>>
{
public:
    SampleStore();
    void add(const Sample &);
};

} // namespace models
} // namespace mimir

#endif // SAMPLESTORE_H
