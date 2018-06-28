#include "DataStore.h"

#include <algorithm>
#include <ios>

#include "../traits/Timing.h"

using std::find_if;
using std::sort;

using std::pair;
using std::string;
using std::vector;

using mimir::models::CPT;
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
    _stride = static_cast<long>(_columnNames.size());
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

CPT DataStore::createConditionalProbabilityTable(std::vector<models::ValueIndex> columns) const
{
    if (columns.size() == 0) {
        columns = _columnNames;
    }
    vector<long> indices;
    // indices (to find columns) and values to match
    for (auto search : columns) {
        indices.push_back(columnByName(search));
    }
    vector<vector<ValueIndex>> rows;
    auto cursor = _rawData.begin();
    while (cursor != _rawData.end()) {
        vector<ValueIndex> row;
        for (auto index : indices) {
            row.push_back(*(cursor + index));
        }
        rows.push_back(row);
        cursor += _stride;
    }
    sort(rows.begin(), rows.end());
    return CPT(columns, rows);
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
