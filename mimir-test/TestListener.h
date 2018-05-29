#ifndef TESTLISTENER_H
#define TESTLISTENER_H

#include <QtCore/qglobal.h>
#include <QTest>

#include <QMap>

class TestListener
{
public:
    inline TestListener() : _tests() {}
    void addTest(const QString &name, QObject*test);
    QMap<QString, QObject*> availableTests() const;
    static TestListener &getListener();
    static void release();
private:
    QMap<QString, QObject*> _tests;
};

template<typename Testee>
class Test {
public:
    QSharedPointer<Testee> _testee;
    Test(const QString &name)
    {
        _testee = QSharedPointer<Testee>(new Testee);
        TestListener::getListener().addTest(name, _testee.data());
    }
};

#define REGISTER_TEST(test) static Test<test> _##test(QStringLiteral(#test));
#endif // TESTLISTENER_H
