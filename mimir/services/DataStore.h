#ifndef DATASTORE_H
#define DATASTORE_H

#include <vector>
#include <map>

#include "../models/ValueIndex.h"
#include "NameResolver.h"
#include "Sampler.h"

namespace mimir {
namespace services {

class DataStore
{
public:
    DataStore(NameResolver &);
    void createDataSet(std::vector<models::ValueIndex> const &);
    void addRow(std::vector<models::ValueIndex>);
    Sampler createSampler(models::ValueIndex classifier, models::ValueIndex value) const;
    size_t columnCount() const;
    size_t rowCount() const;
private:
    long columnByName(models::ValueIndex) const;
private:
    using Column = std::vector<models::ValueIndex>;
    using Table = std::vector<Column>;
    NameResolver &_nameResolver;
    std::vector<models::ValueIndex> _columNames;
    Table _rawData;

};

} // namespace services
} // namespace mimir

#endif // DATASTORE_H
