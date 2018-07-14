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

    mkValueIndex(yes);
    mkValueIndex(no);

    mkValueIndex(hostMale);
    mkValueIndex(hostFemale);

    mkValueIndex(morning);
    mkValueIndex(afternoon);
    mkValueIndex(night);

    _dataStore.createDataSet({"status", "type", "colour", "agentContact", "presenterSex", "sizesAvailable", "dayTime"});

    vector<vector<ValueIndex>> testData = {
        {cancelled, ring,    red,   no,  hostMale,   no,   morning},
        {returned,  brooch,  green, yes, hostFemale, no,   afternoon},
        {kept,      ring,    green, no,  hostMale,   yes,  night},
        {kept,      brooch,  red,   yes, hostFemale, no,   afternoon},
        {kept,      brooch,  blue,  no,  hostMale,   no,   morning},
        {returned,  collier, blue,  yes, hostFemale, yes,  afternoon},
        {returned,  collier, green, no,  hostMale,   yes,  night},
        {cancelled, brooch,  green, yes, hostFemale, no,   afternoon},
        {kept,      ring,    green, no,  hostMale,   no,   night},
        {cancelled, collier, red,   yes, hostFemale, no,   afternoon}
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
            no = _nameResolver.indexFromName("no"),
            yes = _nameResolver.indexFromName("yes"),
            hostMale = _nameResolver.indexFromName("hostMale"),
            status = _nameResolver.indexFromName("status"),
            colour = _nameResolver.indexFromName("colour"),
            ccContact = _nameResolver.indexFromName("agentContact"),
            presenterSex = _nameResolver.indexFromName("presenterSex"),
            type = _nameResolver.indexFromName("type"),
            dayTime = _nameResolver.indexFromName("dayTime"),
            sizesAvailable = _nameResolver.indexFromName("sizesAvailable"),
            afternoon = _nameResolver.indexFromName("afternoon");

    CPT cpt = _dataStore.createConditionalProbabilityTable();
    auto classification = cpt.classify({{type, ring}, {colour, green}, {ccContact, no}, {presenterSex, hostMale}, {sizesAvailable, yes}, {dayTime, afternoon}}, status);
    classification.dump(std::cerr, _nameResolver);

    std::cerr << "------" << std::endl;
    DependencyDetector detect(cpt);

    auto net = detect.findSuitableGraph({{type, ring}, {colour, green}, {ccContact, no}, {presenterSex, hostMale}, {sizesAvailable, yes}, {dayTime, afternoon}});
    for (auto node : net) {
        node.dump(std::cerr, _nameResolver);
    }
    std::cerr << "------" << std::endl;
    //detect.detectDependencies({ValueIndex(ValueIndex::AnyIndex), ring, green, noContact, hostMale}, status);

}

void TestPatternFind::testPredict()
{
}

