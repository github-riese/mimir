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

static void dumpRow(CPT::Row const &row)
{
    std::cout << "| ";
    for(auto v : row.values) {
        std::cout << v << "\t| ";
    }
    std::cout << row.probability << " |" << std::endl;
}
static inline bool matches(CPT::Row const &row, vector<ColumnIndexValuePair> const &rule)
{
    auto match = rule.begin();
    while (match != rule.end()) {
        ValueIndex search = (*match).value;
        if (search != ValueIndex::AnyIndex && search != row.values.at(static_cast<size_t>((*match).columnIndex))) {
            return false;
        }
        ++match;
    }
    return true;
}

struct AccumulateHelper
{
    vector<ColumnIndexValuePair> matchRules;
    Probability operator()(Probability p, CPT::Row const& row)
    {
        if (!matches(row, matchRules)) {
                return p;
        }
        return p + row.probability;
    }
};

CPT::CPT(vector<ValueIndex> fields, vector<vector<ValueIndex>> table) :
    _fields(fields)
{
    calculateProbabilities(table);
}

std::vector<ValueIndex> CPT::fields() const
{
    return _fields;
}

Probability CPT::probability(std::vector<ColumnNameValuePair> values) const
{
    auto matchRules = buildMatchRule(values);
    AccumulateHelper rowMatch{matchRules};
    Probability p = accumulate(_proabilities.begin(), _proabilities.end(), 0._p, rowMatch);
    return p;
}

ProbabilityDistribution CPT::classify(const std::vector<ColumnNameValuePair> &columns, ValueIndex classifier)
{
    traits::VerboseTiming<std::chrono::microseconds> _timing("CPT::classify");
    auto matchRule = buildMatchRule(columns);
    auto classifierIndex = fieldIndex(classifier);
    return classify(matchRule, classifierIndex);
}

ProbabilityDistribution CPT::classify(const std::vector<ColumnIndexValuePair> &matchRule, long classifierIndex)
{
    map<ValueIndex, Probability> result;
    Probability sumOfMatches;
    auto runner = [&](Row const &row) {
        if (!matches(row, matchRule)) {
                return;
        }
        sumOfMatches += row.probability;
        result[row.values.at(static_cast<size_t>(classifierIndex))] += row.probability;
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
                [index, &seen](Row const & value){
        ValueIndex v = value.values.at(static_cast<size_t>(index));
        seen.insert(v);
    });

    result.insert(result.begin(), seen.begin(), seen.end());
    return result;
}

std::ostream& CPT::dump(std::ostream &stream, services::NameResolver &resolver) const
{
    for (auto row : _proabilities) {
        stream << "| ";
        for (auto index : row.values) {
            stream << resolver.nameFromIndex(index) << "\t| ";
        }
        stream << "  " << row.probability.value() << "|" << std::endl;
    }
    return stream;
}

void CPT::calculateProbabilities(vector<vector<ValueIndex>> table)
{
    std::ostringstream ss;
    ss << "CPT::calculateProbabilities of " << table.size() << " values";
    traits::VerboseTiming<std::chrono::microseconds> _timer(ss.str());

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
    assert(accumulate(_proabilities.begin(), _proabilities.end(), 0._p, [](Probability _1, Row const &_2){
        return _1 + _2.probability;
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

vector<ColumnIndexValuePair> CPT::buildMatchRule(const std::vector<ColumnNameValuePair> &values) const
{
    vector<ColumnIndexValuePair> matchRules;
    auto knownColumns = _fields.begin();
    auto valueIdx = values.begin();
    while (valueIdx != values.end()) {
        auto i = find_if(knownColumns, _fields.end(), [&valueIdx](ValueIndex vi) { return vi == (*valueIdx).columnName; });
        matchRules.push_back({distance(knownColumns, i), (*(valueIdx++)).value});
    }
    return matchRules;
}

} // namespace models
} // namespace mimir
