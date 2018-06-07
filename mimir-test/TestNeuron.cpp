#include "TestNeuron.h"

#include <QDebug>

#include "../mimir/models/Probability.h"


#include "../mimir/services/Evaluator.h"

using mimir::models::Evaluation;
using mimir::models::Probability;
using mimir::services::Evaluator;

REGISTER_TEST(TestNeuron)

TestNeuron::TestNeuron(QObject *parent) : QObject(parent)
{

}

void TestNeuron::testConcept()
{
}
