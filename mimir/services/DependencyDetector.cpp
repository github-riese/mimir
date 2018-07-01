#include "DependencyDetector.h"

#include <algorithm>

#include "../models/Probability.h"
#include "../traits/Timing.h"

#include <QDebug>

using std::pair;
using std::vector;

using std::find_if;

using mimir::models::CPT;
using mimir::models::NamedProbability;
using mimir::models::Probability;
using mimir::models::ValueIndex;

namespace mimir {
namespace services {

DependencyDetector::DependencyDetector(CPT &cpt) :
    _cpt(cpt)
{

}

vector<vector<ValueIndex> > DependencyDetector::detectDependencies(vector<ValueIndex> const& values, ValueIndex classifiingColumn)
{
    traits::VerboseTiming<std::chrono::microseconds> _timer("detectDependencies");
    auto colums = _cpt.fields();
    size_t valXnIdx = 0, valXn1Idx = 1;
    vector<vector<ValueIndex>> result;
    auto colXn = colums.begin();
    auto colXn1 = colums.begin() + 1;
    while (colXn != colums.end()) {
        while (colXn1 != colums.end() && colXn != colums.end()) {
            if ((*colXn) == classifiingColumn) {
                ++valXnIdx;
                ++colXn;
                continue;
            }
            if ((*colXn1) == classifiingColumn) {
                ++valXn1Idx;
                ++colXn1;
                continue;
            }
            if (colXn == colXn1) {
                ++valXn1Idx;
                ++colXn1;
                continue;
            }
            Probability pXn = _cpt.probability({{*colXn, values.at(valXnIdx)}});
            if (pXn.isZero()) {
                qDebug() << "ignore column" << valXnIdx << " it's zero";
                ++valXnIdx;
                ++colXn;
                continue;
            }
            Probability pXn1 = _cpt.probability({{*colXn1, values.at(valXn1Idx)}});
            if (pXn1.isZero()) {
                qDebug() << "ignore column" << valXn1Idx << " it's zero";
                ++valXn1Idx;
                ++colXn1;
                continue;
            }

            Probability pXnIfpXn1 = _cpt.probability({{*colXn, values.at(valXnIdx)}, {*colXn1, values.at(valXn1Idx)}});
            if (std::abs(pXn * pXn1 - pXnIfpXn1) <= .05001l) {
                qDebug("Independed: columns %zu %zu (distance %.4Lf)", valXnIdx, valXn1Idx, ((pXn * pXn1) - pXnIfpXn1));
            } else {
                qDebug("dependend:  columns %zu %zu (distance %.4Lf)", valXnIdx, valXn1Idx, ((pXn * pXn1) - pXnIfpXn1));
            }
            ++valXn1Idx;
            ++colXn1;
        }
        colXn1 = colums.begin();
        valXn1Idx = 0;
        ++valXnIdx;
        ++colXn;
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
