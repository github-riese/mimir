#include "TestNeuron.h"

#include <QDebug>
#include <exception>

#include <models/Neuron.h>
#include <models/Layer.h>
#include <models/Net.h>

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
    n1.resetInput();
    n1.addInput(-1);
    n1.addInput(-1);
    QVERIFY(qFuzzyCompare(-.964028f, static_cast<float>(n1.value())));
    n1.resetInput();
    n1.addInput(1);
    n1.addInput(1);
    QVERIFY(qFuzzyCompare(.964028f, static_cast<float>(n1.value())));
}

void TestNeuron::testLayer()
{
    Layer inputLayer;
    inputLayer.addNeuron({});
    inputLayer.addNeuron({});
    auto valueVector = inputLayer.values();
    QCOMPARE((std::vector<double>{0.,0.}), valueVector);
    Layer outputLayer;
    try {
        outputLayer.weight(0, 0);
        QFAIL("Expected an exception but none was thrown");
    } catch (std::exception &e) {
        QCOMPARE(e.what(), "not connected");
    }
    outputLayer.addNeuron({});
    inputLayer.connect(outputLayer);
    auto weight = inputLayer.weight(0, 0);
    QVERIFY(weight == 1.);
    inputLayer.setValues({0, 0});
    inputLayer.run();
    QVERIFY(outputLayer.values() == std::vector<double>{0});
}

void TestNeuron::testOr()
{
    Net orNet(2, 1);
    orNet.addHiddenLayer(17);
    orNet.addHiddenLayer(4);
    orNet.connect();
    auto result = orNet.run({1, 0});
    qDebug() << result;
    result = orNet.run({0, 0});
    qDebug() << result;
    result = orNet.run({0, 1});
    qDebug() << result;
}
