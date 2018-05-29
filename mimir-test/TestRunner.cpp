#include "TestRunner.h"

TestRunner::TestRunner(const TestListener &listener) :
    _listener(listener)
{
}

bool TestRunner::runAll(int argc, char** argv)
{
    bool succeeded = true;
    auto available = _listener.availableTests();
    auto iterator = available.begin();
    while (iterator != available.end()) {
        succeeded &= run(iterator.key(), argc, argv);
        ++iterator;
    }
    return succeeded;
}

bool TestRunner::run(const QString &name, int argc, char **argv)
{
    return QTest::qExec(_listener.availableTests()[name], argc, argv) == 0;
}
