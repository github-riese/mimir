#ifndef TESTSERIALIZE_H
#define TESTSERIALIZE_H

#include <QtCore/QObject>
#include <QtCore/qglobal.h>

class TestSerialize : public QObject
{
    Q_OBJECT
public:
    explicit TestSerialize(QObject *parent = nullptr);

signals:

private slots:
    void testActivationManager();
    void testPackUnpack();
    void testSerialize();
};

#endif // TESTSERIALIZE_H
