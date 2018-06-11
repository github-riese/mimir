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

void TestPatternFind::testPreCheckAssumptionThatDataTurnOutAOne()
{
    Evaluator e;
    ValueIndex kept = _nameResolver.indexFromName("kept");
    Sampler typeSampler = _dataStore.createSampler("status", "type");
    Sampler colourSampler = _dataStore.createSampler("status", "colour");
    Sampler hostSex = _dataStore.createSampler("status", "presenterSex");
    Sampler contactSampler = _dataStore.createSampler("status", "agentContact");

    Sampler colourInTypeSampler = _dataStore.createSampler("colour", "type");

    Evaluation classColourInRingEvaluation = e.evaluate(colourInTypeSampler, _nameResolver.indexFromName("ring"), { _nameResolver.indexFromName("green")});

    Evaluation typeEvaluation = e.evaluate(typeSampler, _nameResolver.indexFromName("ring"));
    Evaluation colourEvaluation = e.evaluate(colourSampler, _nameResolver.indexFromName("green"));
    Evaluation hostSexEvaluation = e.evaluate(hostSex, _nameResolver.indexFromName("hostMale"));
    Evaluation contactEvaluation = e.evaluate(contactSampler, _nameResolver.indexFromName("hadContact"));
    Evaluation combColourType = e.evaluate({ typeEvaluation, colourEvaluation });
    Evaluation combColourHostSex = e.evaluate({colourEvaluation, hostSexEvaluation });
    Evaluation combContactHostSex = e.evaluate({contactEvaluation, hostSexEvaluation});

    ProbabilityWithAPrioris greenInRingProb = classColourInRingEvaluation.probabilityByClassificationEx(_nameResolver.indexFromName("green"));

    ProbabilityWithAPrioris ringProb = typeEvaluation.probabilityByClassificationEx(kept);
    ProbabilityWithAPrioris greenProb = colourEvaluation.probabilityByClassificationEx(kept);
    ProbabilityWithAPrioris maleProb = hostSexEvaluation.probabilityByClassificationEx(kept);
    ProbabilityWithAPrioris contactProb = contactEvaluation.probabilityByClassificationEx(kept);
    ProbabilityWithAPrioris combColourTypeProb = combColourType.probabilityByClassificationEx(kept);
    ProbabilityWithAPrioris combColourHostSexProb = combColourHostSex.probabilityByClassificationEx(kept);
    ProbabilityWithAPrioris combContactHostSexProb = combColourHostSex.probabilityByClassificationEx(kept);

    qDebug("ring:    p: %Lf    pA: %Lf    pB: %Lf", ringProb.probability().probability(), ringProb.aPrioryA().probability(), ringProb.aPrioryB().probability());
    qDebug("green:   p: %Lf    pA: %Lf    pB: %Lf", greenProb.probability().probability(), greenProb.aPrioryA().probability(), greenProb.aPrioryB().probability());
    qDebug("comb:    p: %Lf    pA: %Lf    pB: %Lf", combColourTypeProb.probability().probability(), combColourTypeProb.aPrioryA().probability(), combColourTypeProb.aPrioryB().probability());
    qDebug("resamp:  p: %Lf    pA: %Lf    pB: %Lf", greenInRingProb.probability().probability(), greenInRingProb.aPrioryA().probability(), greenInRingProb.aPrioryB().probability());
    Probability greenRing = (ringProb.aPrioryA() * greenInRingProb.probability() * greenProb.aPrioryA()) /
            (greenInRingProb.aPrioryB());
    qDebug("Prob that kept when green given it was a ring: %Lf, green: %Lf, ring: %Lf",
           greenRing.probability(),
           greenProb.probability().probability(),
           ringProb.probability().probability());

    Probability greenByMaleHost = (maleProb.aPrioryB() * ringProb.aPrioryA())/combColourHostSexProb.aPrioryA();
    qDebug("Prob that kept when a male presented given it was green: %Lf, host: %Lf",
           greenByMaleHost.probability(),
           maleProb.probability().probability());
    qDebug("Size of subgroup was %Lf", combColourHostSexProb.aPrioryA().probability());

    Probability maleHostAndContact = (maleProb.aPrioryA() * contactProb.aPrioryB())/combContactHostSexProb.aPrioryB();
    qDebug("HostMale: %Lf, had contact: %Lf, combined: %Lf, graphed: %Lf",
           maleProb.probability().probability(),
           contactProb.probability().probability(),
           combContactHostSexProb.probability().probability(),
           maleHostAndContact.probability());

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

