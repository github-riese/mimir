#ifndef TESTTRAINER_H
#define TESTTRAINER_H

#include <QtCore/QObject>
#include <QtCore/qglobal.h>

class TestTrainer : public QObject
{
    Q_OBJECT
public:
    explicit TestTrainer(QObject *parent = nullptr);

signals:

private slots:
    void testXOR();
    void testChangeNet();
    void testTrain();
    void testImageDetect();
};

#endif // TESTTRAINER_H
