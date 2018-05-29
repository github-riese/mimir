#ifndef TESTRUNNER_H
#define TESTRUNNER_H

#include <QtCore/qglobal.h>
#include "TestListener.h"

class TestRunner
{
public:
    TestRunner(const TestListener &listener);
    bool runAll(int argc, char **argv);
    bool run(const QString &, int argc, char** argv);
private:
    TestListener _listener;
};

#endif // TESTRUNNER_H
