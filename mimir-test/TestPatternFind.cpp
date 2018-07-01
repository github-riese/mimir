#include "TestPatternFind.h"
#include <QDebug>

#include "../mimir/models/CPT.h"
#include "../mimir/models/ProbabilityWithPriors.h"
#include "../mimir/services/Evaluator.h"
#include "../mimir/services/DependencyDetector.h"

using std::vector;

using mimir::models::CPT;
using mimir::models::Evaluation;
using mimir::models::ColumnNameValuePair;
using mimir::models::Probability;
using mimir::models::ProbabilityWithPriors;
using mimir::models::ValueIndex;

using mimir::services::DependencyDetector;
using mimir::services::Evaluator;
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

    _dataStore.createDataSet({"status", "type", "colour", "agentContact", "presenterSex"});

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
    ValueIndex ring = _nameResolver.indexFromName("ring"),
            green = _nameResolver.indexFromName("green"),
            noContact = _nameResolver.indexFromName("noContact"),
            hostMale = _nameResolver.indexFromName("hostMale"),
            status = _nameResolver.indexFromName("status"),
            colour = _nameResolver.indexFromName("colour"),
            ccContact = _nameResolver.indexFromName("agentContact"),
            presenterSex = _nameResolver.indexFromName("presenterSex"),
            type = _nameResolver.indexFromName("type");

    CPT cpt = _dataStore.createConditionalProbabilityTable();
    auto classification = cpt.classify({{type, ring}, {colour, green}, {ccContact, noContact}, {presenterSex, hostMale}}, status);
    classification.dump(std::cerr, _nameResolver);

    DependencyDetector detect(cpt);
    detect.detectDependencies({ValueIndex(ValueIndex::AnyIndex), ring, green, noContact, hostMale}, status);

}

void TestPatternFind::testPredict()
{
}

