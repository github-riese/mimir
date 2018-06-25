#include "TestPatternFind.h"
#include <QDebug>

#include "../mimir/models/ProbabilityWithPriors.h"
#include "../mimir/services/Evaluator.h"
#include "../mimir/services/EvaluationCombiner.h"

using std::vector;

using mimir::models::Evaluation;
using mimir::models::Probability;
using mimir::models::ProbabilityWithPriors;
using mimir::models::ValueIndex;

using mimir::services::Evaluator;
using mimir::services::EvaluationCombiner;
using mimir::services::NameResolver;

#define mkValueIndex(name) ValueIndex name = _nameResolver.indexFromName(#name);
TestPatternFind::TestPatternFind() :
    _dataStore(_nameResolver)
{

}

void TestPatternFind::initTestCase()
{
    ValueIndex kept = _nameResolver.indexFromName("kept");
    ValueIndex cancelled = _nameResolver.indexFromName("cancelled");
    ValueIndex returned = _nameResolver.indexFromName("returned");

    mkValueIndex(ring);
    mkValueIndex(collier);
    mkValueIndex(brooch);

    mkValueIndex(blue);
    mkValueIndex(red);
    mkValueIndex(green);

    mkValueIndex(hadContact);
    mkValueIndex(noContact);

    mkValueIndex(hostMale);
    mkValueIndex(hostFemale);

    _dataStore.createDataSet({"status", "type", "colour", "agentContact", "presenterSex"}, "status");

    vector<vector<ValueIndex>> testData = {
        {cancelled, ring,    red,   noContact,  hostMale},
        {returned,  brooch,  green, hadContact, hostFemale},
        {kept,      ring,    green, noContact,  hostMale},
        {kept,      brooch,  red,   hadContact, hostFemale},
        {kept,      brooch,  blue,  noContact,  hostMale},
        {returned,  collier, blue, hadContact, hostFemale},
        {returned,  collier, green, noContact,  hostMale},
        {cancelled, brooch,  green, hadContact, hostFemale},
        {kept,      ring,    green, noContact,  hostMale},
        {cancelled, collier, red,  hadContact, hostFemale}
    };

    for (auto row : testData) {
        _dataStore.addRow(row);
    }
}

QString dumpProb(const char*name, ProbabilityWithPriors const &prob)
{
    QString txt(name);
    if (txt.length()) {
        txt = txt.left(6);
        txt += QString().fill(' ', 7 - txt.length());
    }
    txt += QString().append("p(class|val): %1 p(class): %2 p(val): %3 likelyhood: %4").
            arg(static_cast<double>(prob.probability().value()), 8, 'f', 6).
            arg(static_cast<double>(prob.prior().value()), 8, 'f', 6).
            arg(static_cast<double>(prob.evidence().value()), 8, 'f', 6).
            arg(static_cast<double>(prob.likelihood().value()), 8, 'f', 6);
    return txt;
}

QDebug &operator<<(QDebug &debug, const ProbabilityWithPriors &p){
    debug << dumpProb("", p);
    return debug;
}

void TestPatternFind::testPreCheckAssumptionThatDataTurnOutAOne()
{
}

void TestPatternFind::testPredict()
{
}

