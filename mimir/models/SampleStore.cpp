#include "SampleStore.h"

#include <numeric>

using std::accumulate;

namespace mimir {
namespace models {

SampleStore::SampleStore()
{

}

void SampleStore::add(const Sample &sample)
{
    auto &valueCounters = (*this)[sample.classifier()];
    auto item = find_if(valueCounters.begin(), valueCounters.end(), [&sample](ValueCounter const &v){
            return v.value() == sample.value();
    });
    if(item == valueCounters.end()) {
        valueCounters.push_back(ValueCounter(sample.value(), sample.count()));
    } else {
        (*item) += sample.count();
    }
}

unsigned SampleStore::total() const
{
    unsigned total = 0;
    for (auto classes : (*this)) {
        total += accumulate(classes.second.cbegin(), classes.second.cend(), 0);
    }
    return total;
}

unsigned SampleStore::countInValue(size_t value) const
{
    unsigned total = 0;
    for (auto classes : (*this)) {
        for (auto values : classes.second) {
            if (values.value() == value) {
                total += values.count();
            }
        }
    }
    return total;
}

unsigned SampleStore::countInClass(size_t classifier) const
{
    auto classPtr = find(classifier);
    if (classPtr == end()) {
        return 0;
    }
    return accumulate((*classPtr).second.begin(), (*classPtr).second.end(), 0);
}

unsigned SampleStore::count(size_t classifier, size_t value) const
{
    unsigned total = 0;
    auto classPtr = find(classifier);
    if (classPtr == end()) {
        return 0;
    }
    for (auto values : (*classPtr).second) {
        if (values.value() == value) {
            total += values.count();
        }
    }
    return total;
}

} // namespace models
} // namespace mimir
