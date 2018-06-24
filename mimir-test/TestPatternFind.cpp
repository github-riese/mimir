#include "TestPatternFind.h"
#include <QDebug>

#include "../mimir/models/ProbabilityWithPriors.h"
#include "../mimir/services/Evaluator.h"
#include "../mimir/services/EvaluationCombiner.h"
#include "../mimir/services/Sampler.h"

using std::vector;

using mimir::models::Evaluation;
using mimir::models::Probability;
using mimir::models::ProbabilityWithPriors;
using mimir::models::Sample;
using mimir::models::ValueIndex;

using mimir::services::Evaluator;
using mimir::services::EvaluationCombiner;
using mimir::services::NameResolver;
using mimir::services::Sampler;

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
    Evaluator e;
    ValueIndex kept = _nameResolver.indexFromName("kept");
    ValueIndex cancelled = _nameResolver.indexFromName("cancelled");
    ValueIndex returned = _nameResolver.indexFromName("returned");
    Sampler typeSampler = _dataStore.createSampler("status", "type");
    Sampler colourSampler = _dataStore.createSampler("status", "colour");

    Sampler colourInTypeSampler = _dataStore.createSampler("colour", "type");

    Evaluation classColourInRingEvaluation = e.evaluate(colourInTypeSampler, _nameResolver.indexFromName("ring"), { _nameResolver.indexFromName("green")});

    Evaluation typeEvaluation = e.evaluate(typeSampler, _nameResolver.indexFromName("ring"));
    Evaluation colourEvaluation = e.evaluate(colourSampler, _nameResolver.indexFromName("green"));
    Evaluation combColourType = e.classify({ typeEvaluation, colourEvaluation });

    ProbabilityWithPriors greenInRingProb = classColourInRingEvaluation.probabilityByClassificationEx(_nameResolver.indexFromName("green"));

    ProbabilityWithPriors ringProb = typeEvaluation.probabilityByClassificationEx(kept);
    ProbabilityWithPriors greenProb = colourEvaluation.probabilityByClassificationEx(kept);


    qDebug() << dumpProb("ring", ringProb);
    qDebug() << dumpProb("green", greenProb);
    qDebug() << dumpProb("resamp", greenInRingProb);

    qDebug() << "kept in combined"  << combColourType.probabilityByClassificationEx(kept);
    qDebug() << "cancelled in combined"  << combColourType.probabilityByClassificationEx(cancelled);
    qDebug() << "returned in combined"  << combColourType.probabilityByClassificationEx(returned);
    QCOMPARE(static_cast<double>(combColourType.probabilityByClassification(kept)), static_cast<double>(Probability(2.l/3)));
    qDebug() << e.opcount() << "operations";
}

void TestPatternFind::testPredict()
{
    EvaluationCombiner combiner(_dataStore);
    Evaluator e;

    Sampler typeSampler = _dataStore.createSampler("status", "type");
    Sampler colourSampler = _dataStore.createSampler("status", "colour");
    Sampler ccContactSampler = _dataStore.createSampler("status", "agentContact");
    Sampler hostSexSampler = _dataStore.createSampler("status", "presenterSex");
    Evaluation typeEvaluation = e.evaluate(typeSampler, _nameResolver.indexFromName("ring"));
    Evaluation colourEvaluation = e.evaluate(colourSampler, _nameResolver.indexFromName("green"));
    Evaluation ccContact = e.evaluate(ccContactSampler, _nameResolver.indexFromName("hadContact"));
    Evaluation hostSex = e.evaluate(hostSexSampler, _nameResolver.indexFromName("hostFemale"));

    combiner.addEvaluations({typeEvaluation, colourEvaluation, ccContact, hostSex});
    unsigned passesUsed = combiner.findClusters();
    QCOMPARE(4u, passesUsed);
}

