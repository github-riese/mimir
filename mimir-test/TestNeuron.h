#ifndef TESTNEURON_H
#define TESTNEURON_H

#include <QtCore/QObject>
#include <QtCore/qglobal.h>

#include "TestListener.h"

class TestNeuron : public QObject
{
    Q_OBJECT
public:
    explicit TestNeuron(QObject *parent = nullptr);

signals:

private Q_SLOTS:
    void testConcept();
};

#endif // TESTNEURON_H
