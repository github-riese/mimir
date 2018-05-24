#include <QString>
#include <QtTest>

#include "../mimir/models/SampleStore.h"

using mimir::models::Sample;
using mimir::models::SampleStore;

class Models : public QObject
{
    Q_OBJECT

public:
    Models();

private Q_SLOTS:
    void testCase1();
};

Models::Models()
{
}

void Models::testCase1()
{
    SampleStore testee;
    testee.add(Sample{1, 2});
    testee.add(Sample{1, 2, 3});
    testee.add(Sample{2, 2, 99});
    testee.add(Sample{1, 1, 5});
    QCOMPARE(testee.total(), 99U + 1 + 3 + 5);
    QCOMPARE(testee.countInValue(2), 99U + 3 + 1);
    QCOMPARE(testee.count(1, 2), 1U + 3);
    QCOMPARE(testee.count(2, 1), 0U);
    QCOMPARE(testee.countInClass(2), 99U);
}

QTEST_APPLESS_MAIN(Models)

#include "tst_Models.moc"
