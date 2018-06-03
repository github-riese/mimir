#include "TestRunner.h"
#include <QElapsedTimer>
#include <QDebug>

TestRunner::TestRunner(const TestListener &listener) :
    _listener(listener)
{
}

bool TestRunner::runAll(int argc, char** argv)
{
    bool succeeded = true;
    QElapsedTimer timer;
    auto available = _listener.availableTests();
    auto iterator = available.begin();
    timer.start();
    unsigned testCount = 0;
    while (iterator != available.end()) {
        succeeded &= run(iterator.key(), argc, argv);
        ++iterator;
        ++testCount;
    }
    qDebug() << "Ran" << testCount << "tests in" << timer.elapsed() << "ms";
    return succeeded;
}

bool TestRunner::run(const QString &name, int argc, char **argv)
{
    return QTest::qExec(_listener.availableTests()[name], argc, argv) == 0;
}
