#include "TestListener.h"

static TestListener *_testListener;

void TestListener::addTest(const QString &name, QObject *test)
{
    if (_tests.find(name) != _tests.end()) {
        _tests[name]->deleteLater();
    }
    _tests[name] = test;
}

QMap<QString, QObject *> TestListener::availableTests() const
{
    return _tests;
}

TestListener &TestListener::getListener()
{
    if (_testListener == nullptr) {
        _testListener = new TestListener;
    }
    return *_testListener;
}

void TestListener::release()
{
    TestListener *theInstance = _testListener;
    _testListener = nullptr;
    delete theInstance;
}
