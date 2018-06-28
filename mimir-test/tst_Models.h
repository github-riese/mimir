#ifndef TST_MODELS_H
#define TST_MODELS_H

#include <QObject>

#include "../mimir/services/DataStore.h"
#include "../mimir/services/NameResolver.h"

class Models : public QObject
{
    Q_OBJECT

public:
    Models();

private Q_SLOTS:
    void initTestCase();
    void testDataStore();
    void testNameLookup();
    void testHelpers();
    void testCPT();
private:
    mimir::services::NameResolver _nameResolver;
    mimir::services::DataStore _dataStore;
};

#endif // TST_MODELS_H
