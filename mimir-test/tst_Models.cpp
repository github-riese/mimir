#include <QString>
#include <QtTest>

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

class Models : public QObject
{
    Q_OBJECT

public:
    Models();

private Q_SLOTS:
    void testSamplerBasics();
    void testNameLookup();
    void testProbabilator();
};

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

    QCOMPARE(vKept, ValueIndex(0));
    QCOMPARE(vKept2, ValueIndex(0));
    QCOMPARE(vCancelled, ValueIndex(1));
    QCOMPARE(vReturned, ValueIndex(2));
    QCOMPARE(vPropertyColour, ValueIndex(0));
    QCOMPARE(vPropertySize, ValueIndex(1));
    QCOMPARE(vValueBlue, ValueIndex(0));
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

    Probabilator probS1("s1probabilator");
    probS1.setSampler(s1);
    Evaluation eS1 = probS1.evaluateSampler(1_vi);
    QCOMPARE(eS1.mostProbableClass(), 1_vi);
    Probability pS1 = eS1.mostProbable();

    QCOMPARE(pS1.probability(), 5.L/8);

    Probabilator probS2("s2probabilator");
    probS2.setSampler(s2);
    Evaluation eS2 = probS2.evaluateSampler(3_vi);
    QCOMPARE(eS2.mostProbableClass(), 1_vi);
    Probability pS2 = eS2.mostProbable();

    QCOMPARE(pS2.probability(), 4.L/5);

    Probabilator combineP1P2("p1p2Combinator");
    Probability combinedP1P2 = combineP1P2.combineProbabilities({pS1, pS2});
    qDebug() << "Prob: " << static_cast<double>(combinedP1P2.probability()) << "Expected: 20/32 ->" << 20./32;
    QCOMPARE(combinedP1P2.probability(), 20./32);

    auto expectedSamplerIndices = vector<vector<ValueIndex>>{{1_vi}, {2_vi}};
    QCOMPARE(combinedP1P2.samplers(), expectedSamplerIndices);
}

QTEST_APPLESS_MAIN(Models)

#include "tst_Models.moc"
