#include "SampleStore.h"

namespace mimir {
namespace models {

SampleStore::SampleStore()
{

}

void SampleStore::add(const Sample &sample)
{
    auto &valueCounters = (*this)[sample.classifier()];
    auto item = find_if(valueCounters.begin(), valueCounters.end(), [&sample](ValueCounter const &v){
            return v.valueIndex() == sample.valueIndex();
    });
    if(item == valueCounters.end()) {
        valueCounters.push_back(ValueCounter(sample.valueIndex(), sample.count()));
    } else {
        (*item) += sample.count();
    }
}

} // namespace models
} // namespace mimir
