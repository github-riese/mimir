#include "DataStore.h"

#include <algorithm>
#include <ios>

#include "../traits/Timing.h"

using std::find_if;

using std::string;
using std::vector;

using mimir::models::Sample;
using mimir::models::ValueIndex;

namespace mimir {
namespace services {

DataStore::DataStore(NameResolver &nameResolver) :
    _nameResolver(nameResolver)
{

}

void DataStore::createDataSet(std::vector<std::string> columnNames, std::string classifiingColumn)
{
    vector<ValueIndex> columnNameIndices;
    for_each(columnNames.begin(), columnNames.end(), [&columnNameIndices, this](const string &name){
        columnNameIndices.push_back(_nameResolver.indexFromName(name));
    });
    createDataSet(columnNameIndices, _nameResolver.indexFromName(classifiingColumn));
}

void DataStore::createDataSet(const vector<ValueIndex> &columnNames, ValueIndex classifiingColumn)
{
    _rawData.clear();
    _columNames.clear();
    _columNames = columnNames;
    _stride = _columNames.size();
    _classifyingColumn = classifiingColumn;
}

void DataStore::addRow(vector<ValueIndex> row)
{
    if (row.size() != _stride) {
        throw "Bad data size";
    }
    _rawData.insert(_rawData.end(), row.begin(), row.end());
    ++_rows;
}

Sampler DataStore::createSampler(ValueIndex classifier, ValueIndex value) const
{
    Sampler sampler(value);
    long classIndex = columnByName(classifier);
    long valIndex = columnByName(value);
    if (classIndex == -1 || valIndex == -1) {
        return Sampler();
    }
    auto classClmnIdx = _rawData.begin() + classIndex;
    auto valClmnIdx = _rawData.begin() + valIndex;

    int stride = static_cast<int>(_stride);
    size_t samplesToCreate = _rows;
    traits::VerboseTiming<std::chrono::nanoseconds> t(std::string("create sampler ") + _nameResolver.nameFromIndex(classifier) + "-" + _nameResolver.nameFromIndex(value));
    while (samplesToCreate-- > 0) {
        sampler.addSample(Sample(*classClmnIdx, *valClmnIdx));
        classClmnIdx += stride;
        valClmnIdx += stride;
    }
    return sampler;
}

Sampler DataStore::createSampler(const std::string &classifier, const std::string &value) const
{
    return createSampler(_nameResolver.indexFromName(classifier), _nameResolver.indexFromName(value));
}

size_t DataStore::columnCount() const
{
    return _columNames.size();
}

size_t DataStore::rowCount() const
{
    return _rawData.size() == 0 ? 0 :
                                  _rawData.size() / _stride;
}

models::ValueIndex DataStore::classifyingColumn() const
{
    return _classifyingColumn;
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
