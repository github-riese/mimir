#include "CPT.h"

#include <algorithm>
#include <assert.h>
#include <iterator>
#include <numeric>
#include <map>
#include <set>
#include <sstream>

#include <traits/Timing.h>

using std::map;
using std::pair;
using std::set;
using std::string;
using std::vector;

using std::accumulate;
using std::unique_copy;
using std::distance;
using std::find_if;

namespace mimir {
namespace models {

static void dumpRow(pair<vector<ValueIndex>, Probability> const &row)
{
    std::cout << "| ";
    for(auto v : row.first) {
        std::cout << v << "\t| ";
    }
    std::cout << row.second << " |" << std::endl;
}
static inline bool matches(pair<vector<ValueIndex>, Probability> const &row, vector<pair<long int, ValueIndex>> const &rule)
{
    auto match = rule.begin();
    while (match != rule.end()) {
        ValueIndex search = match->second;
        if (search != ValueIndex::AnyIndex && search != row.first.at(static_cast<size_t>(match->first))) {
            return false;
        }
        ++match;
    }
    return true;
}

struct AccumulateHelper
{
    vector<pair<long int, ValueIndex>> matchRules;
    Probability operator()(Probability p, pair<vector<ValueIndex>, Probability> row)
    {
        if (!matches(row, matchRules)) {
                return p;
        }
        return p + row.second;
    }
};

CPT::CPT(vector<ValueIndex> fields, vector<vector<ValueIndex>> table) :
    _fields(fields)
{
    calculateProbabilities(table);
}

Probability CPT::probability(vector<ValueIndex> columns, vector<ValueIndex> values) const
{
    auto matchRules = buildMatchRule(columns, values);
    AccumulateHelper rowMatch{matchRules};
    Probability p = accumulate(_proabilities.begin(), _proabilities.end(), 0._p, rowMatch);
    return p;
}

Probability CPT::evidence(const std::vector<ValueIndex> &row)
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

ProbabilityDistribution CPT::classify(const vector<ValueIndex> &columns, const vector<ValueIndex> &values, ValueIndex classifier)
{
    traits::VerboseTiming<std::chrono::nanoseconds>("CPT::classify");
    auto matchRule = buildMatchRule(columns, values);
    auto classifierIndex = fieldIndex(classifier);
    map<ValueIndex, Probability> result;
    Probability sumOfMatches;
    auto runner = [&](pair<vector<ValueIndex>, Probability> const &row) {
        if (!matches(row, matchRule)) {
                return;
        }
        sumOfMatches += row.second;
        result[row.first.at(static_cast<size_t>(classifierIndex))] += row.second;
        dumpRow(row);
    };
    for_each(_proabilities.begin(), _proabilities.end(), runner);
    Probability scale = 1/sumOfMatches;
    for_each(result.begin(), result.end(), [scale](auto &value) {
        value.second *= scale;
    });
    return ProbabilityDistribution(result);
}

vector<ValueIndex> CPT::distinctValues(ValueIndex field) const
{
    vector<ValueIndex> result;
    long int index = fieldIndex(field);
    if (index == -1) {
        return result;
    }

    set<ValueIndex> seen;
    for_each(_proabilities.begin(), _proabilities.end(),
                [index, &seen](pair<vector<ValueIndex>, Probability> const & value){
        ValueIndex v = value.first.at(static_cast<size_t>(index));
        seen.insert(v);
    });

    result.insert(result.begin(), seen.begin(), seen.end());
    return result;
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

long CPT::fieldIndex(ValueIndex name) const
{
    auto field = find_if(_fields.begin(), _fields.end(), [name](ValueIndex item){ return  item == name; });
    if (field == _fields.end()) {
        return -1;
    }
    return distance(_fields.begin(), field);
}

vector<pair<long, ValueIndex>> CPT::buildMatchRule(std::vector<ValueIndex> columns, std::vector<ValueIndex> values) const
{
    vector<pair<long int, ValueIndex>> matchRules;
    auto knownColumns = _fields.begin();
    auto valueIdx = values.begin();
    for (auto column : columns) {
        auto i = find_if(knownColumns, _fields.end(), [&column](ValueIndex vi) { return vi == column; });
        matchRules.push_back({distance(knownColumns, i), *(valueIdx++)});
    }
    return matchRules;
}

} // namespace models
} // namespace mimir
