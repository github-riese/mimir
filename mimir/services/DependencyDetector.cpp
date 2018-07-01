#include "DependencyDetector.h"

#include <algorithm>

#include "../models/Probability.h"
#include "../traits/Timing.h"

#include <QDebug>

using std::pair;
using std::vector;

using std::find_if;

using mimir::models::CPT;
using mimir::models::ColumnIndexValuePair;
using mimir::models::NamedProbability;
using mimir::models::Probability;
using mimir::models::ValueIndex;

namespace mimir {
namespace services {

DependencyDetector::DependencyDetector(CPT &cpt) :
    _cpt(cpt)
{

}

vector<vector<ValueIndex> > DependencyDetector::detectDependencies(vector<ValueIndex> const& values, ValueIndex classifiingColumn, NameResolver&ns)
{
    traits::VerboseTiming<std::chrono::microseconds> _timer("detectDependencies");
    auto fields = _cpt.fields();
    vector<ColumnIndexValuePair> independent;
    auto classifyingIndex = _cpt.fieldIndex(classifiingColumn);
    vector<ColumnIndexValuePair> candidates;
    for (auto field : fields) {
        auto idx = _cpt.fieldIndex(field);
        if (idx == classifyingIndex) {
            continue;
        }
        candidates.push_back({idx, values.at(static_cast<size_t>(idx))});
    }
    vector<vector<ValueIndex>> result;
    auto candidate = candidates.begin();
    while (candidate != candidates.end()) {
        auto partner = candidate + 1;
        while (partner != candidates.end()) {
            if (partner == candidate) {
                ++partner;
                continue;
            }
            Probability pThisGivenThat = conditionalProbability(*candidate, {*partner});
            Probability pThatGivenThis = conditionalProbability(*partner, {*(candidate)});
            Probability p1 = _cpt.probability({*candidate});
            Probability p2 = _cpt.probability({*partner});
            std::cerr << "P(" << ns.nameFromIndex(fields.at(candidate->columnIndex)) << '=' << ns.nameFromIndex(candidate->value) << '|'
                      << ns.nameFromIndex(fields.at((partner)->columnIndex)) << '=' << ns.nameFromIndex((partner)->value) << "): "
                      << pThisGivenThat
                      << ", "
                      << "P(" << ns.nameFromIndex(fields.at((partner)->columnIndex)) << '=' << ns.nameFromIndex((partner)->value) << '|'
                      << ns.nameFromIndex(fields.at((candidate)->columnIndex)) << '=' << ns.nameFromIndex((candidate)->value) << "): "
                      << pThatGivenThis
                      << "\t P(a) * P(b): " << p1 * p2
                      << std::endl;
            ++partner;
        }
        ++candidate;
    }
    return result;
}

models::Probability DependencyDetector::conditionalProbability(const models::ColumnIndexValuePair &classifier, const std::vector<models::ColumnIndexValuePair> &fields)
{
    auto result = _cpt.classify(fields, classifier.columnIndex);
    auto classes = result.distribution();
    auto v = find_if(classes.begin(), classes.end(), [&classifier](const NamedProbability& p){
        return p.name == classifier.value;
    });
    return v == classes.end() ? 0._p : (*v).probability;
}

} // namespace services
} // namespace mimir
