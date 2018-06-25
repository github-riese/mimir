#include "DataStore.h"

#include <algorithm>
#include <ios>

#include "../traits/Timing.h"

using std::find_if;

using std::string;
using std::vector;

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
    _columnNames.clear();
    _columnNames = columnNames;
    _stride = _columnNames.size();
    _classifyingColumn = classifiingColumn;
}

void DataStore::addRow(vector<ValueIndex> row)
{
    if (row.size() != static_cast<size_t>(_stride)) {
        throw "Bad data size";
    }
    _rawData.insert(_rawData.end(), row.begin(), row.end());
    ++_rows;
}

DataStore DataStore::intersect(const std::vector<models::ValueIndex> &columns, ValueIndex classifier, ValueIndex classValue) const
{
    traits::VerboseTiming<std::chrono::microseconds> _timer("DataStore::intersect");
    DataStore result(_nameResolver);
    result.createDataSet(columns, classifier);
    auto iterator = _rawData.begin();
    auto classifierIndex = columnByName(classifier);
    vector<long> indices;
    for (auto c : columns) {
        indices.push_back(columnByName(c));
    }
    while (iterator != _rawData.end()) {
        if (*(iterator + classifierIndex) != classValue) {
            iterator += _stride;
            continue;
        }
        vector<ValueIndex> newRow;
        newRow.reserve(columns.size());
        for (auto index : indices) {
            newRow.push_back(*(iterator + index));
        }
        result.addRow(newRow);
        iterator += _stride;
    }
    return result;
}

size_t DataStore::columnCount() const
{
    return _columnNames.size();
}

size_t DataStore::rowCount() const
{
    return _rawData.size() == 0 ? 0 :
                                  _rawData.size() / _columnNames.size();
}

models::ValueIndex DataStore::classifyingColumn() const
{
    return _classifyingColumn;
}

long DataStore::columnByName(ValueIndex name) const
{
    auto index = find_if(_columnNames.begin(), _columnNames.end(), [name](ValueIndex const &i){
            return i == name;
    });
    if (index == _columnNames.end()) {
        return -1;
    }
    return index - _columnNames.begin();
}

} // namespace services
} // namespace mimir
