#ifndef DATASTORE_H
#define DATASTORE_H

#include <deque>
#include <map>
#include <string>
#include <vector>

#include "../models/CPT.h"
#include "../models/Probability.h"
#include "../models/ValueIndex.h"
#include "NameResolver.h"

namespace mimir {
namespace services {

class DataStore
{
public:
    DataStore(NameResolver &);
    void createDataSet(std::vector<std::string>);
    void createDataSet(std::vector<models::ValueIndex> const &);
    void addRow(std::vector<models::ValueIndex>);
    models::CPT createConditionalProbabilityTable(std::vector<models::ValueIndex> columns = std::vector<models::ValueIndex>()) const;
    size_t columnCount() const;
    size_t rowCount() const;
    inline std::vector<models::ValueIndex> const & columnNames() const { return _columnNames; }
private:
    long columnByName(models::ValueIndex) const;
private:
    NameResolver &_nameResolver;
    std::vector<models::ValueIndex> _columnNames;
    std::deque<models::ValueIndex> _rawData;
    long _stride = 0;
    size_t _rows = 0;
};

} // namespace services
} // namespace mimir

#endif // DATASTORE_H
