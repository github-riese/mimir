#include "TestRunner.h"
#include <QDebug>

#include <iotaomegapsi/tools/timer/Timing.h>

using iotaomegapsi::tools::timer::VerboseTiming;

TestRunner::TestRunner(const TestListener &listener) :
    _listener(listener)
{
}

bool TestRunner::runAll(int argc, char** argv)
{
    bool succeeded = true;
    auto available = _listener.availableTests();
    auto iterator = available.begin();
    VerboseTiming<std::chrono::milliseconds> timer("All Tests");
    unsigned testCount = 0;
    while (iterator != available.end()) {
        VerboseTiming<std::chrono::microseconds> testTimer(std::string("test: ") + iterator.key().toUtf8().data());
        succeeded &= run(iterator.key(), argc, argv);
        ++iterator;
        ++testCount;
    }
    qDebug() << "Ran" << testCount << "tests";
    return succeeded;
}

bool TestRunner::run(const QString &name, int argc, char **argv)
{
    return QTest::qExec(_listener.availableTests()[name], argc, argv) == 0;
}
