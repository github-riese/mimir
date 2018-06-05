#ifndef TESTPATTERNFIND_H
#define TESTPATTERNFIND_H

#include <vector>
#include <QtCore/qglobal.h>
#include <QObject>

#include "../mimir/models/ValueType.h"
#include "../mimir/services/NameResolver.h"
#include "../mimir/services/Sampler.h"

#include "TestListener.h"

class TestPatternFind : public QObject
{
    Q_OBJECT
public:
    TestPatternFind();
private Q_SLOTS:
    void initTestCase();
    void testPreCheckAssumptionThatDataTurnOutAOne();
    void testPreCheckOnEvaluator();
private:
private:
    mimir::services::NameResolver _nameResolver;
    std::vector<mimir::services::Sampler> _testSamplers;
};

REGISTER_TEST(TestPatternFind)
#endif // TESTPATTERNFIND_H
