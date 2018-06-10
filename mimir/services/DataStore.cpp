#include "DataStore.h"

#include <algorithm>
#include <ios>

#include "../traits/Timing.h"

using std::find_if;

using std::vector;

using mimir::models::Sample;
using mimir::models::ValueIndex;

namespace mimir {
namespace services {

DataStore::DataStore(NameResolver &nameResolver) :
    _nameResolver(nameResolver)
{

}

void DataStore::createDataSet(const vector<ValueIndex> &columnNames)
{
    _rawData.clear();
    _columNames.clear();
    _columNames = columnNames;
    _rawData.resize(_columNames.size());
}

void DataStore::addRow(vector<ValueIndex> row)
{
    auto inColumns = row.begin();
    auto storeColumns = _rawData.begin();
    while (inColumns != row.end() && storeColumns != _rawData.end()) {
        storeColumns->push_back(*inColumns);
        ++inColumns;
        ++storeColumns;
    }
}

Sampler DataStore::createSampler(ValueIndex classifier, ValueIndex value) const
{
    Sampler sampler(value);
    long classIndex = columnByName(classifier);
    long valIndex = columnByName(value);
    if (classIndex == -1 || valIndex == -1) {
        return Sampler();
    }
    traits::VerboseTiming t(std::string("create sampler ") + _nameResolver.nameFromIndex(classifier) + "-" + _nameResolver.nameFromIndex(value));
    auto &classifications = _rawData.at(static_cast<size_t>(classIndex));
    auto &values = _rawData.at(static_cast<size_t>(valIndex));

    auto classClmnIdx = classifications.begin();
    auto valClmnIdx = values.begin();

    while (classClmnIdx != classifications.end() && valClmnIdx != values.end()) {
        sampler.addSample(Sample(*classClmnIdx, *valClmnIdx));
        ++classClmnIdx;
        ++valClmnIdx;
    }
    return sampler;
}

size_t DataStore::columnCount() const
{
    return _columNames.size();
}

size_t DataStore::rowCount() const
{
    return _rawData.size() == 0 ? 0 :
                 (*(_rawData.begin())).size();
}

long DataStore::columnByName(ValueIndex name) const
{
    auto index = find_if(_columNames.begin(), _columNames.end(), [name](ValueIndex const &i){
            return i == name;
    });
    if (index == _columNames.end()) {
        return -1;
    }
    return index - _columNames.begin();
}

} // namespace services
} // namespace mimir
