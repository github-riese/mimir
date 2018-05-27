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
    NameResolver nameResolve;
    nameResolve.indexFromName(NameResolver::NameSource::Sampler, "a totally unused sampler name");
    ValueIndex testSamplerName = nameResolve.indexFromName(NameResolver::NameSource::Sampler, "testsampler1");
    Sampler testSampler1(testSamplerName);
    testSampler1.addSample(Sample{1_vi, 1_vi, 10});
    testSampler1.addSample(Sample{2_vi, 1_vi, 10});
    testSampler1.addSample(Sample{3_vi, 1_vi, 10});
    testSampler1.addSample(Sample{1_vi, 2_vi, 10});
    testSampler1.addSample(Sample{2_vi, 2_vi, 10});
    testSampler1.addSample(Sample{3_vi, 2_vi, 10});
    testSampler1.addSample(Sample{1_vi, 3_vi, 10});
    testSampler1.addSample(Sample{2_vi, 3_vi, 10});
    testSampler1.addSample(Sample{3_vi, 3_vi, 10});

    Sampler testSampler2(nameResolve.indexFromName(NameResolver::NameSource::Sampler, "testSampler2"));
    testSampler2.addSample(Sample(1_vi, 1_vi, 5));
    testSampler2.addSample(Sample(2_vi, 1_vi, 2));

    Probabilator testee("testee");
    testee.addSampler(testSampler1);

    Evaluation e1 = testee.evaluate(1_vi);

    Probability p = e1.probabilityByClassification(1_vi);
    QCOMPARE(p.probability(), 1.l/3); // probability of a third to find 1 in class 1
    QCOMPARE(p.totalSamples(), 90U);
    QCOMPARE(p.samplers(), { testSamplerName });

    testee.addSampler(testSampler2);

    Evaluation e2 = testee.evaluate(1_vi);

    Probability p2 = e2.probabilityByClassification(1_vi);
    vector<ValueIndex> expectation2 = {1_vi, 2_vi };

    long double combined = p2.probability();
    QCOMPARE(p2.samplers(), expectation2);

    Probabilator prob2("xx");
    prob2.addSampler(testSampler2);
    Evaluation prob2Evaluation = prob2.evaluate(1_vi);
    Probability prob2P = prob2Evaluation.probabilityByClassification(1_vi);
    Probability metacombined = testee.metaProbability({p, prob2P});
    QCOMPARE(combined, metacombined.probability());
    QCOMPARE(p2.samplers(), metacombined.samplers());
}

QTEST_APPLESS_MAIN(Models)

#include "tst_Models.moc"
