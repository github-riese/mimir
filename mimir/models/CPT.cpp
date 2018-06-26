#include "CPT.h"

#include <algorithm>
#include <assert.h>
#include <iterator>
#include <numeric>
#include <sstream>

#include <traits/Timing.h>

using std::pair;
using std::string;
using std::vector;

using std::accumulate;
using std::distance;
using std::find_if;

namespace mimir {
namespace models {

CPT::CPT(vector<vector<ValueIndex>> table)
{
    calculateProbabilities(table);
}

Probability CPT::probability(const std::vector<ValueIndex> &row)
{
    auto found = find_if(_proabilities.begin(), _proabilities.end(),
                     [&row](pair<vector<ValueIndex>, Probability> const &value) {
                        return row == value.first;
                    });
    if (found == _proabilities.end()) {
        return 0._p;
    }
    return (*found).second;
}

std::ostream& CPT::dump(std::ostream &stream, services::NameResolver &resolver) const
{
    for (auto row : _proabilities) {
        stream << "| ";
        for (auto index : row.first) {
            stream << resolver.nameFromIndex(index) << "\t| ";
        }
        stream << "  " << row.second.value() << "|" << std::endl;
    }
    return stream;
}

void CPT::calculateProbabilities(vector<vector<ValueIndex>> table)
{
    std::ostringstream ss;
    ss << "CPT::calculateProbabilities of " << table.size() << " values";
    traits::VerboseTiming<std::chrono::microseconds>(ss.str());

    auto total = table.size();
    auto current = table.begin();
    auto previous = current;
    while (++current != table.end()) {
        if (*current == *previous) {
            continue;
        }
        _proabilities.push_back({*previous, Probability(static_cast<long double>(distance(previous, current))/total)});
        previous = current;
    }
    _proabilities.push_back({*previous, Probability(static_cast<long double>(distance(previous, current))/total)});
    assert(accumulate(_proabilities.begin(), _proabilities.end(), 0._p, [](Probability _1, pair<vector<ValueIndex>, Probability> const &_2){
        return _1 + _2.second;
    }) == 1._p);
}

} // namespace models
} // namespace mimir
