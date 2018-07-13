#include "NetworkFragment.h"

namespace mimir {
namespace models {


NetworkFragment::NetworkFragment(ColumnNameValuePair classifier, std::vector<ColumnNameValuePair> parents, Probability probability) :
    _value(classifier),
    _parents(parents),
    _probability(probability)
{
}

ColumnNameValuePair NetworkFragment::value() const
{
    return _value;
}

std::vector<ColumnNameValuePair> NetworkFragment::parents() const
{
    return _parents;
}

Probability NetworkFragment::probability() const
{
    return _probability;
}

std::ostream &NetworkFragment::dump(std::ostream &stream, services::NameResolver &nameResolver)
{
    stream << "P("
           << nameResolver.nameFromIndex(_value.columnName)
           << '='
           << nameResolver.nameFromIndex(_value.value);
    if (_parents.size() > 0) {
        stream << '|';
        bool first = true;
        for (auto v : _parents) {
            if (first) first = false;
            else stream << ", ";
            stream << nameResolver.nameFromIndex(v.columnName) << "=" << nameResolver.nameFromIndex(v.value);
        }
    }
    stream << ") = ";
    stream << _probability;
    stream << std::endl;
    return stream;
}

} // namespace models
} // namespace mimir
