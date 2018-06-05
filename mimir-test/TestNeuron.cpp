#include "TestNeuron.h"

#include <QDebug>

#include "../mimir/models/Probability.h"

#include "../mimir/models/neurons/ActivationFunctionCombinedProbabilities.h"

#include "../mimir/services/Evaluator.h"

using mimir::models::Evaluation;
using mimir::models::Probability;
using mimir::models::neurons::ActivationFunctionCombinedProbabilities;
using mimir::services::Evaluator;

REGISTER_TEST(TestNeuron)

TestNeuron::TestNeuron(QObject *parent) : QObject(parent)
{

}

void TestNeuron::testConcept()
{
    Probability a(.3L, .5L, .2L, {{1_vi}});
    Probability b(.6L, .5L, .375L, {{2_vi}});

    long double mean = (a.probability() + b.probability()) /2;
    long double deviation = (abs(a.probability() - mean) + abs(b.probability() - mean))/2;

    Evaluator e;
    Probability combined = e.combineProbabilities({a, b});

    ActivationFunctionCombinedProbabilities activator(mean, deviation, combined);
    bool doesntActivate = activator();
    QCOMPARE(doesntActivate, false);

    Probability c(.6L, .5L, .3L, {{3_vi}});
    Probability combined2 = e.combineProbabilities({a, c});
    ActivationFunctionCombinedProbabilities activator2(mean, deviation, combined2);

    bool activates = activator2();
    QCOMPARE(activates, true);
}
