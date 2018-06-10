#ifndef TST_MODELS_H
#define TST_MODELS_H

#include <QObject>

class Models : public QObject
{
    Q_OBJECT

public:
    Models();

private Q_SLOTS:
    void testSamplerBasics();
    void testDataStore();
    void testNameLookup();
    void testProbabilator();
    void testHelpers();
};

#endif // TST_MODELS_H
