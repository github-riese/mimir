#ifndef TESTPATTERNFIND_H
#define TESTPATTERNFIND_H

#include <vector>
#include <QtCore/qglobal.h>
#include <QObject>

#include "../mimir/services/DataStore.h"
#include "../mimir/services/NameResolver.h"

#include "TestListener.h"

class TestPatternFind : public QObject
{
    Q_OBJECT
public:
    TestPatternFind();
    ~TestPatternFind();
private Q_SLOTS:
    void initTestCase();
    void testPreCheckAssumptionThatDataTurnOutAOne();
    void testPredict();
private:
private:
    mimir::services::NameResolver _nameResolver;
    mimir::services::DataStore _dataStore;
};

#endif // TESTPATTERNFIND_H
