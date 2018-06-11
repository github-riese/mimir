#ifndef DATASTORE_H
#define DATASTORE_H

#include <deque>
#include <map>
#include <string>
#include <vector>

#include "../models/ValueIndex.h"
#include "NameResolver.h"
#include "Sampler.h"

namespace mimir {
namespace services {

class DataStore
{
public:
    DataStore(NameResolver &);
    void createDataSet(std::vector<std::string>);
    void createDataSet(std::vector<models::ValueIndex> const &);
    void addRow(std::vector<models::ValueIndex>);
    Sampler createSampler(models::ValueIndex classifier, models::ValueIndex value) const;
    Sampler createSampler(std::string const& classifier, std::string const &value) const;
    size_t columnCount() const;
    size_t rowCount() const;
private:
    long columnByName(models::ValueIndex) const;
private:
    NameResolver &_nameResolver;
    std::vector<models::ValueIndex> _columNames;
    std::deque<models::ValueIndex> _rawData;
    size_t _stride = 0;
    size_t _rows = 0;
};

} // namespace services
} // namespace mimir

#endif // DATASTORE_H
