#ifndef TESTMATRIX_H
#define TESTMATRIX_H

#include <QtCore/qglobal.h>
#include <QtCore/QObject>

class TestMatrix : public QObject
{
    Q_OBJECT
public:
    TestMatrix();
private slots:
    void testEquals();
    void testTranspose();
    void testMultiply();
    void testOtherMultiplications();
    void testDiff();
    void testCallbackConstructor();
    void testInsertRow();
};

#endif // TESTMATRIX_H
