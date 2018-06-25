#include "Sample.h"

using std::vector;
using std::pair;

namespace mimir {
namespace models {

Sample::Sample(ValueIndex classifier, std::vector<ValueCounter> vals) :
    _classifier(classifier),
    _values(vals)
{
}

Sample::Sample(ValueIndex classifier, vector<ValueIndex> values) :
    _classifier(classifier),
    _values()
{
    for (auto v : values) {
        _values.push_back(ValueCounter{v, 1});
    }
}

Sample::Sample(ValueIndex classifier, ValueIndex val, unsigned long count) :
    _classifier(classifier),
    _values(ValueCounter{val, count})
{

}

ValueIndex Sample::classifier() const
{
    return _classifier;
}

std::vector<ValueCounter> Sample::values() const
{
    return _values;
}


} // namespace models
} // namespace mimir
