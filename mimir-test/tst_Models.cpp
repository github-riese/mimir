#include <QString>
#include <QtTest>

#include <deque>

#include "TestListener.h"

#include "tst_Models.h"

#include "../mimir/models/ValueType.h"
#include "../mimir/services/DataStore.h"
#include "../mimir/services/Evaluator.h"
#include "../mimir/services/NameResolver.h"
#include "../mimir/services/Sampler.h"

#include "../mimir/helpers/helpers.h"

using mimir::models::Evaluation;
using mimir::models::Probability;
using mimir::models::ProbabilityWithAPrioris;
using mimir::models::Sample;
using mimir::models::ValueType;
using mimir::models::ValueIndex;
using mimir::services::DataStore;
using mimir::services::Evaluator;
using mimir::services::NameResolver;
using mimir::services::Sampler;

REGISTER_TEST(Models)

Models::Models()
{
}

void Models::testSamplerBasics()
{
    NameResolver resolver;
    ValueIndex testeeNameIndex = resolver.indexFromName("testee");
    Sampler testee(testeeNameIndex);
    testee.addSample(Sample{ValueIndex(1), ValueIndex(2)});
    testee.addSample(Sample{ValueIndex(1), ValueIndex(2), 3});
    testee.addSample(Sample{ValueIndex(2), ValueIndex(2), 99});
    testee.addSample(Sample{ValueIndex(1), ValueIndex(1), 5});
    QCOMPARE(testee.total(), 99U + 1 + 3 + 5);
    QCOMPARE(testee.countInValue(ValueIndex(2)), 99U + 3 + 1);
    QCOMPARE(testee.count(ValueIndex(1), ValueIndex(2)), 1U + 3);
    QCOMPARE(testee.count(ValueIndex(2), ValueIndex(1)), 0U);
    QCOMPARE(testee.countInClass(ValueIndex(2)), 99U);
}

void Models::testDataStore()
{
    NameResolver nameResolver;
    #define mkValueIndex(name) ValueIndex name = nameResolver.indexFromName(#name);
    ValueIndex kept = nameResolver.indexFromName("kept");
    ValueIndex cancelled = nameResolver.indexFromName("cancelled");
    ValueIndex returned = nameResolver.indexFromName("returned");

    mkValueIndex(ring);
    mkValueIndex(collier);
    mkValueIndex(brooch);

    mkValueIndex(blue);
    mkValueIndex(red);
    mkValueIndex(green);
    mkValueIndex(yellow);

    mkValueIndex(hadContact);
    mkValueIndex(noContact);

    mkValueIndex(hostMale);
    mkValueIndex(hostFemale);

    mkValueIndex(classification);
    mkValueIndex(type);
    mkValueIndex(colour);
    mkValueIndex(ccContact);
    mkValueIndex(hostSex);

    #undef mkValueIndex

    vector<vector<ValueIndex>> testData = {
        {cancelled, ring, blue, noContact, hostMale},
        {kept, ring, green, hadContact, hostFemale},
        {kept, ring, green, noContact, hostMale},
        {kept, brooch, red, hadContact, hostFemale},
        {kept, brooch, blue, noContact, hostMale},
        {returned, collier, green, hadContact, hostFemale},
        {returned, ring, red, noContact, hostMale},
        {cancelled, brooch, green, hadContact, hostFemale},
        {kept, ring, green, noContact, hostMale},
        {cancelled, ring, yellow, hadContact, hostFemale}
    };

    DataStore dataStore(nameResolver);
    QVERIFY(dataStore.columnCount() == 0);
    QVERIFY(dataStore.rowCount() == 0);
    dataStore.createDataSet({classification, type, colour, ccContact, hostSex});
    QVERIFY(dataStore.columnCount() == 5);
    QVERIFY(dataStore.rowCount() == 0);

    for (auto row : testData)
        dataStore.addRow(row);
    QVERIFY(dataStore.columnCount() == 5);
    QVERIFY(dataStore.rowCount() == 10);

    Sampler s1 = dataStore.createSampler(classification, type);
    QCOMPARE(s1.total(), 10ul);
    QCOMPARE(s1.allClasses().size(), 3ul);
    QCOMPARE(s1.count(kept, ring), 3ul);
    QCOMPARE(s1.countInClass(returned), 2ul);
    QCOMPARE(s1.countInValue(brooch), 3ul);

    Sampler samplerToFindGreenRings = dataStore.createSampler(colour, type);
    QCOMPARE(samplerToFindGreenRings.total(), 10ul);
    QCOMPARE(samplerToFindGreenRings.allClasses().size(), 4ul);
    QCOMPARE(samplerToFindGreenRings.count(green, ring), 3ul);
}

void Models::testNameLookup()
{
    NameResolver testee;
    ValueIndex vKept = testee.indexFromName("kept"); // #0
    ValueIndex vCancelled = testee.indexFromName("cancelled"); // #1
    ValueIndex vReturned = testee.indexFromName("returned"); // #2

    ValueIndex vPropertyColour = testee.indexFromName("colour"); // #3
    ValueIndex vPropertySize = testee.indexFromName("size"); // #4

    ValueIndex vValueBlue = testee.indexFromName("blue"); // #5

    ValueIndex vKept2 = testee.indexFromName("kept"); // #0 (created above)

    ValueIndex vSampler1 = testee.indexFromName("Sampler1"); // #6
    ValueIndex vColor = testee.indexFromName("colour"); // #3

    QCOMPARE(vKept, ValueIndex(0));
    QCOMPARE(vKept2, ValueIndex(0));
    QCOMPARE(vCancelled, ValueIndex(1));
    QCOMPARE(vReturned, ValueIndex(2));
    QCOMPARE(vPropertyColour, ValueIndex(3));
    QCOMPARE(vPropertySize, ValueIndex(4));
    QCOMPARE(vValueBlue, ValueIndex(5));
    QVERIFY(vSampler1 == 6_vi);
    QVERIFY(vColor == 3_vi);

    QVERIFY(testee.nameFromIndex(3_vi) == "colour");
}

void Models::testProbabilator()
{

    Sampler s1(1_vi);
    Sampler s2(2_vi);
    s1.addSample({1_vi, 1_vi, 5L});
    s1.addSample({2_vi, 1_vi, 1L});
    s1.addSample({3_vi, 1_vi, 2L});
    s1.addSample({1_vi, 2_vi, 5L});
    s1.addSample({2_vi, 2_vi, 1L});
    s1.addSample({3_vi, 2_vi, 2L});

    s2.addSample({1_vi, 3_vi, 4L});
    s2.addSample({3_vi, 3_vi, 1L});
    s2.addSample({1_vi, 4_vi, 1L});
    s2.addSample({2_vi, 4_vi, 2L});
    s2.addSample({3_vi, 4_vi, 2L});

    Evaluator evaluator;
    Evaluation eS1 = evaluator.evaluate(s1, 1_vi);
    QVERIFY2(eS1.mostProbableClass() == 1_vi, QStringLiteral("Expected 1 to be the more probable class.").toUtf8().data());

    Probability pS1 = eS1.mostProbable();
    QCOMPARE(pS1.value(), 5.L/8);
    ProbabilityWithAPrioris pES1 = eS1.probabilityByClassificationEx(eS1.mostProbableClass());
    QCOMPARE(static_cast<double>(pES1.likelyHood().value()), 1./2.);
    QCOMPARE(static_cast<double>(pES1.classProbability().value()), 5./8.);
    QCOMPARE(static_cast<double>(pES1.valueProbability().value()), 1./2.);

    Evaluation eS2 = evaluator.evaluate(s2, 3_vi);
    QCOMPARE(eS2.mostProbableClass(), 1_vi);
    ProbabilityWithAPrioris pES2 = eS2.probabilityByClassificationEx(1_vi);
    QCOMPARE(static_cast<double>(pES2.probability().value()), 4./5.);
    QCOMPARE(static_cast<double>(pES2.likelyHood().value()), 4./5.);
    QCOMPARE(static_cast<double>(pES2.classProbability().value()), 5./10.);
    QCOMPARE(static_cast<double>(pES2.valueProbability().value()), 5./10.);

    Evaluation combinedP1P2 = evaluator.evaluate({eS1, eS2});
    ProbabilityWithAPrioris pC = combinedP1P2.probabilityByClassificationEx(1_vi);
    QCOMPARE(static_cast<double>(pC.probability().value()), 18./65.);

    auto expectedSamplerIndices = vector<vector<ValueIndex>>{{1_vi}, {2_vi}};
    QCOMPARE(combinedP1P2.samplers(), expectedSamplerIndices);

    Evaluation pS1_2S2 = evaluator.evaluate({combinedP1P2, eS2});
    QVERIFY2((pS1_2S2.samplers() == vector<vector<ValueIndex>>{{1_vi, 2_vi}, {2_vi}}), "SamplerCombination mismatch");
    QCOMPARE(static_cast<double>(pS1_2S2.mostProbable().value()), 5./8.);
    qDebug() << "evaluator did " << evaluator.opcount() << "operations";
}

void Models::testHelpers()
{
    vector<Probability> v1 = {
        (.1_p),
        (.2_p),
        (.3_p),
        (.4_p),
        (.5_p),
    };
    long double mean = mimir::helpers::mean(v1);
    QCOMPARE(static_cast<double>(mean), ((.1 + .2 + .3 + .4 + .5)/5.));

    std::deque<Probability> d1;
    d1.insert(d1.end(), v1.begin(), v1.end());
    auto deviation = mimir::helpers::deviation(d1);
    QCOMPARE(static_cast<double>(deviation), .6/5.);
}
