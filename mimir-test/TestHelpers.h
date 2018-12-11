#ifndef TESTHELPERS_H
#define TESTHELPERS_H

#include <QtCore/QObject>
#include <QtCore/qglobal.h>

#include "TestListener.h"

class TestHelpers : public QObject
{
    Q_OBJECT
public:
    explicit TestHelpers(QObject *parent = nullptr);

signals:

private slots:
    void testDiffVectors();
    void testMultiply();
    void testMatrix();
};

#endif // TESTHELPERS_H
