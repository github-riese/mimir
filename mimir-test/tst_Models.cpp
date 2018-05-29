#include <QString>
#include <QtTest>

#include "TestListener.h"

#include "tst_Models.h"

#include "../mimir/services/NameResolver.h"
#include "../mimir/services/Probabilator.h"
#include "../mimir/services/Sampler.h"

using mimir::models::Evaluation;
using mimir::models::Probability;
using mimir::models::Sample;
using mimir::models::ValueIndex;
using mimir::services::NameResolver;
using mimir::services::Sampler;
using mimir::services::Probabilator;

REGISTER_TEST(Models)

Models::Models()
{
}

void Models::testSamplerBasics()
{
    NameResolver resolver;
    ValueIndex testeeNameIndex = resolver.indexFromName(NameResolver::NameSource::Sampler, "testee");
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

void Models::testNameLookup()
{
    NameResolver testee;
    ValueIndex vKept = testee.indexFromName(NameResolver::NameSource::Classification, "kept");
    ValueIndex vCancelled = testee.indexFromName(NameResolver::NameSource::Classification, "cancelled");
    ValueIndex vReturned = testee.indexFromName(NameResolver::NameSource::Classification, "returned");

    ValueIndex vPropertyColour = testee.indexFromName(NameResolver::NameSource::Property, "colour");
    ValueIndex vPropertySize = testee.indexFromName(NameResolver::NameSource::Property, "size");

    ValueIndex vValueBlue = testee.indexFromName(NameResolver::NameSource::Value, "blue");

    ValueIndex vKept2 = testee.indexFromName(NameResolver::NameSource::Classification, "kept");

    ValueIndex vSampler1 = testee.indexFromName(NameResolver::NameSource::Sampler, "Sampler1");
    ValueIndex vColor = testee.indexFromName(NameResolver::NameSource::Sampler, "colour");

    QCOMPARE(vKept, ValueIndex(0));
    QCOMPARE(vKept2, ValueIndex(0));
    QCOMPARE(vCancelled, ValueIndex(1));
    QCOMPARE(vReturned, ValueIndex(2));
    QCOMPARE(vPropertyColour, ValueIndex(0));
    QCOMPARE(vPropertySize, ValueIndex(1));
    QCOMPARE(vValueBlue, ValueIndex(0));
    QVERIFY(vSampler1 == 0_vi);
    QVERIFY(vColor == 1_vi);

    QVERIFY(testee.nameFromIndex(NameResolver::NameSource::Sampler, 1_vi) == "colour");
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

    Probabilator probS1;
    Evaluation eS1 = probS1.evaluate(s1, 1_vi);
    QVERIFY2(eS1.mostProbableClass() == 1_vi, QStringLiteral("Expected 1 to be the more probable class.").toUtf8().data());

    Probability pS1 = eS1.mostProbable();
    QCOMPARE(pS1.probability(), 5.L/8);

    Evaluation eS2 = Probabilator::evaluate(s2, 3_vi);
    QCOMPARE(eS2.mostProbableClass(), 1_vi);
    Probability pS2 = eS2.probabilityByClassification(1_vi);

    QCOMPARE(pS2.probability(), 4.L/5);

    Evaluation combinedP1P2 = Probabilator::evaluate({eS1, eS2});
    QCOMPARE(combinedP1P2.mostProbable().probability(), 20./32);

    auto expectedSamplerIndices = vector<vector<ValueIndex>>{{1_vi}, {2_vi}};
    QCOMPARE(combinedP1P2.mostProbable().samplers(), expectedSamplerIndices);

    Evaluation pS1_2S2 = probS1.evaluate({combinedP1P2, eS2});
    QVERIFY2((pS1_2S2.mostProbable().samplers() == vector<vector<ValueIndex>>{{1_vi, 2_vi}, {2_vi}}), "SamplerCombination mismatch");
}
