#include "NetworkFragment.h"

namespace mimir {
namespace models {


NetworkFragment::NetworkFragment(ColumnNameValuePair input, std::vector<ColumnNameValuePair> parents, Probability probability) :
    _input(input),
    _parents(parents),
    _probability(probability)
{
}

ColumnNameValuePair NetworkFragment::input() const
{
    return _input;
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
           << nameResolver.nameFromIndex(_input.columnName)
           << '='
           << nameResolver.nameFromIndex(_input.value);
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
