#include "TestRunner.h"

int main(int argc, char**argv)
{
    TestRunner testRunner(TestListener::getListener());
    int result = testRunner.runAll(argc, argv) ? 0 : 1;
    TestListener::release();
    return result;
}
