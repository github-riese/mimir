#include "CPT.h"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <numeric>
#include <map>
#include <set>
#include <sstream>

#include <traits/Timing.h>
#include "../helpers/AccumulateHelper.h"

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

CPT::CPT(vector<ValueIndex> fields, vector<vector<ValueIndex>> table) :
    _fieldNames(fields)
{
    calculateProbabilities(table);
}

std::vector<ValueIndex> CPT::fieldNames() const
{
    return _fieldNames;
}

Probability CPT::probability(std::vector<ColumnNameValuePair> values) const
{
    auto matchRules = buildMatchRule(values);
    return probability(matchRules);
}

Probability CPT::probability(std::vector<ColumnIndexValuePair> matchRules) const
{
    helpers::AccumulateHelper rowMatch{matchRules};
    Probability p = accumulate(_proabilities.begin(), _proabilities.end(), 0._p, rowMatch);
    return p;
}

ProbabilityDistribution CPT::classify(ValueIndex classifier, const std::vector<ColumnNameValuePair> &columns)
{
    traits::VerboseTiming<std::chrono::microseconds> _timing("CPT::classify");
    auto matchRule = buildMatchRule(columns);
    auto classifierIndex = fieldIndex(classifier);
    return classify(classifierIndex, matchRule);
}

ProbabilityDistribution CPT::classify(long classifierIndex, const std::vector<ColumnIndexValuePair> &input)
{
    map<ValueIndex, Probability> result;
    Probability sumOfMatches;
    for(auto row : _proabilities){
        if (!row.matchesInput(input)) {
            continue;
        }
        sumOfMatches += row.probability;
        result[row.values.at(static_cast<size_t>(classifierIndex))] += row.probability;
    }
    Probability scale = 1/sumOfMatches;
    for_each(result.begin(), result.end(), [scale](auto &value) {
        value.second *= scale;
    });
    ProbabilityDistribution probabilityDistribution(result);
    return probabilityDistribution;
}

vector<ValueIndex> CPT::distinctValues(ValueIndex field) const
{
    long int index = fieldIndex(field);
    if (index == -1) {
        return vector<ValueIndex>();
    }
    return distinctValues(index);
}

std::vector<ValueIndex> CPT::distinctValues(long fieldIndex) const
{
    vector<ValueIndex> result;
    set<ValueIndex> seen;
    for_each(_proabilities.begin(), _proabilities.end(),
                [fieldIndex, &seen](Row const & value){
        ValueIndex v = value.values.at(static_cast<size_t>(fieldIndex));
        seen.insert(v);
    });

    result.insert(result.end(), seen.begin(), seen.end());
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
    auto field = find_if(_fieldNames.begin(), _fieldNames.end(), [name](ValueIndex item){ return  item == name; });
    if (field == _fieldNames.end()) {
        return -1;
    }
    return distance(_fieldNames.begin(), field);
}

ValueIndex CPT::fieldName(long idx) const
{
    if (0 > idx || _fieldNames.size() < static_cast<size_t>(idx)) {
        return ValueIndex(ValueIndex::NoIndex);
    }
    return _fieldNames.at(static_cast<size_t>(idx));
}

vector<ColumnIndexValuePair> CPT::buildMatchRule(const std::vector<ColumnNameValuePair> &searchFields) const
{
    vector<ColumnIndexValuePair> matchRules;
    auto knownColumn = _fieldNames.begin();
    auto searchField = searchFields.begin();
    while (searchField != searchFields.end()) {
        auto i = find_if(knownColumn, _fieldNames.end(), [&searchField](ValueIndex valueIndex) { return valueIndex == (*searchField).columnName; });
        matchRules.push_back({ distance(knownColumn, i), (*searchField).value });
        ++searchField;
    }
    return matchRules;
}

bool CPT::Row::matchesInput(const std::vector<ColumnIndexValuePair> &input) const
{
    for (auto rule : input) {
        ValueIndex search = rule.value;
        if (search != ValueIndex::AnyIndex && search != values.at(static_cast<size_t>(rule.columnIndex)))
            return false;
    }
    return true;
}

} // namespace models
} // namespace mimir
