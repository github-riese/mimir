#include "TestNeuron.h"

#include <QDebug>
#include <exception>

#include <models/Neuron.h>
#include <models/Layer.h>
#include <models/Net.h>

using mimir::models::Edge;
using mimir::models::Layer;
using mimir::models::Net;
using mimir::models::Neuron;

REGISTER_TEST(TestNeuron)

TestNeuron::TestNeuron(QObject *parent) : QObject(parent)
{

}

void TestNeuron::testNeuronBasics()
{
    Neuron n1;
    Neuron n2;
    QCOMPARE(2, n2.id());
    n1.setBias(0);
    n1.addInput(0);
    n1.addInput(0);
    QCOMPARE(0, n1.value());
    n1.reset();
    n1.addInput(-1);
    n1.addInput(-1);
    QVERIFY(qFuzzyCompare(-.964028f, static_cast<float>(n1.value())));
    n1.reset();
    n1.addInput(1);
    n1.addInput(1);
    QVERIFY(qFuzzyCompare(.964028f, static_cast<float>(n1.value())));
}

void TestNeuron::testLayer()
{
    Layer inputLayer;
    inputLayer.addNeuron(std::make_shared<Neuron>());
    inputLayer.addNeuron(std::make_shared<Neuron>());
    auto valueVector = inputLayer.values();
    QCOMPARE((std::vector<double>{0.,0.}), valueVector);
    Layer outputLayer;
    try {
        outputLayer.edge(0, 0);
        QFAIL("Expected an exception but none was thrown");
    } catch (std::exception &e) {
        QCOMPARE(e.what(), "not connected");
    }
    outputLayer.addNeuron(std::make_shared<Neuron>());
    inputLayer.connect(outputLayer);
    Edge e = inputLayer.edge(0, 0);
    QVERIFY(e.weight() == 1.);
    auto edges = inputLayer[0];
    QVERIFY(edges.size() == 1);
}

void TestNeuron::testOr()
{
    Net orNet(2, 1);
    orNet.addHiddenLayer(17);
    orNet.connect();
    auto result = orNet.run({1, 0});
    result = orNet.run({0, 0});

}
