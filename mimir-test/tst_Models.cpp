#include <QString>
#include <QtTest>

#include "../mimir/services/NameResolver.h"
#include "../mimir/services/Probabilator.h"
#include "../mimir/services/Sampler.h"

using mimir::models::Sample;
using mimir::services::NameResolver;
using mimir::services::Sampler;
using mimir::services::Probabilator;
using mimir::models::ValueIndex;

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
    Sampler testee("testee");
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
    Sampler testSampler1("testsampler1");
    testSampler1.addSample(Sample{1_vi, 1_vi, 10});
    testSampler1.addSample(Sample{2_vi, 1_vi, 10});
    testSampler1.addSample(Sample{3_vi, 1_vi, 10});
    testSampler1.addSample(Sample{1_vi, 2_vi, 10});
    testSampler1.addSample(Sample{2_vi, 2_vi, 10});
    testSampler1.addSample(Sample{3_vi, 2_vi, 10});
    testSampler1.addSample(Sample{1_vi, 3_vi, 10});
    testSampler1.addSample(Sample{2_vi, 3_vi, 10});
    testSampler1.addSample(Sample{3_vi, 3_vi, 10});

    Probabilator testee("testee");
    testee.addSampler(testSampler1);

    QCOMPARE(testee.evaluate(1_vi, 1_vi).probability(), 1.l/3);
}

QTEST_APPLESS_MAIN(Models)

#include "tst_Models.moc"
