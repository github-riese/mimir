#include "TestPatternFind.h"
#include <QDebug>

#include "../mimir/models/ProbabilityWithAPrioris.h"
#include "../mimir/services/Evaluator.h"
#include "../mimir/services/EvaluationCombiner.h"
#include "../mimir/services/Sampler.h"

using std::vector;

using mimir::models::Evaluation;
using mimir::models::Probability;
using mimir::models::ProbabilityWithAPrioris;
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

Probability recalcB_in_A(ProbabilityWithAPrioris pris)
{
    return pris.likelyHood();
    // p = (p(b|a) * p(a)) / p(b)     | / p(b|a)
    // (p / p(b|a) = p(a) / p(b)      | / p
    // 1 / p(b|a) = p(a) / p(b) * p
    // p(b|a) = (p(b) * p) / p(a)
//    if (pris.probOfClass().isZero()) {
//        return 0;
//    }
//    return (pris.probOfValue() * pris.probability()) / pris.probOfClass();
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

QString dumpProb(const char*name, ProbabilityWithAPrioris const &prob)
{
    QString txt(name);
    txt = txt.left(6);
    txt += QString().fill(' ', 7 - txt.length());
    txt += QString().append("p(class|val): %1 p(class): %2 p(val): %3 p(val|class): %4").
            arg(static_cast<double>(prob.probability().value()), 8, 'f', 6).
            arg(static_cast<double>(prob.probOfClass().value()), 8, 'f', 6).
            arg(static_cast<double>(prob.probOfValue().value()), 8, 'f', 6).
            arg(static_cast<double>(recalcB_in_A(prob).value()), 8, 'f', 6);
    return txt;
}

void TestPatternFind::testPreCheckAssumptionThatDataTurnOutAOne()
{
    Evaluator e;
    ValueIndex kept = _nameResolver.indexFromName("kept");
    Sampler typeSampler = _dataStore.createSampler("status", "type");
    Sampler colourSampler = _dataStore.createSampler("status", "colour");

    Sampler colourInTypeSampler = _dataStore.createSampler("colour", "type");

    Evaluation classColourInRingEvaluation = e.evaluate(colourInTypeSampler, _nameResolver.indexFromName("ring"), { _nameResolver.indexFromName("green")});

    Evaluation typeEvaluation = e.evaluate(typeSampler, _nameResolver.indexFromName("ring"));
    Evaluation colourEvaluation = e.evaluate(colourSampler, _nameResolver.indexFromName("green"));
    Evaluation combColourType = e.evaluate({ typeEvaluation, colourEvaluation });

    ProbabilityWithAPrioris greenInRingProb = classColourInRingEvaluation.probabilityByClassificationEx(_nameResolver.indexFromName("green"));

    ProbabilityWithAPrioris ringProb = typeEvaluation.probabilityByClassificationEx(kept);
    ProbabilityWithAPrioris greenProb = colourEvaluation.probabilityByClassificationEx(kept);
    ProbabilityWithAPrioris combColourTypeProb = combColourType.probabilityByClassificationEx(kept);


    qDebug() << dumpProb("ring", ringProb);
    qDebug() << dumpProb("green", greenProb);
    qDebug() << dumpProb("comb", combColourTypeProb);
    qDebug() << dumpProb("resamp", greenInRingProb);
    qDebug();

    qDebug("Doing: P(V1|C) * P(V1|V2) * P(C|V1) * P(V2|V1)");
    qDebug("       ---------------------------------------");
    qDebug("                 P(C) * P(V2)");
    qDebug();
    qDebug("with");
    qDebug("        %Lf * %Lf * %Lf * %Lf",
           recalcB_in_A(ringProb).value(),
           recalcB_in_A(greenInRingProb).value(),
           ringProb.probability().value(),
           greenInRingProb.probability().value());
    qDebug("        --------------------------------------");
    qDebug("               %Lf * %Lf",
           ringProb.probOfClass().value(),
           greenProb.probOfValue().value());
    qDebug();
    Probability combined = recalcB_in_A(ringProb) * recalcB_in_A(greenInRingProb) * ringProb.probability() * greenInRingProb.probability();
    combined /= ringProb.probOfClass() * greenProb.probOfValue();
    qDebug("P(V1 | C, V2): %.9Lf", combined.value());
    qDebug() << "and now: P(V1|C,V2) * P(C|V2)";
    qDebug() << "         --------------------";
    qDebug() << "               P(V1|V2)";
    qDebug();
    Probability result = (combined * greenProb.probability()) / greenInRingProb.probability();
    qDebug(" with: (%Lf * %Lf) / %Lf",
           combined.value(),
           greenProb.probability().value(),
           (1- greenInRingProb.probability().value()));
    qDebug();
    qDebug("results in %Lf", result.value());
    qDebug("or simpler: P(keep|ring)/P(green|ring): %Lf",
           (ringProb.probability()/
           greenInRingProb.probability()).value());
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

