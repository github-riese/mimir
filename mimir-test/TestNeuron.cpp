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

template <typename T>
static bool valArraysEqual(std::valarray<T> const &left, std::valarray<T> const &right)
{
    if (left.size() != right.size()) {
        return false;
    }
    return (left == right).min() == true;
}

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
    QVERIFY((std::vector<double>{0.,0.} == valueVector));
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
    inputLayer.addInput({0, 0});
    inputLayer.run();
    QVERIFY((outputLayer.values() == std::vector<double>{0}));
    inputLayer.addInput({2., 1.});
    auto inputRead = inputLayer.input();
    QVERIFY(inputRead[0] == 2.);
    QVERIFY(inputRead[1] == 1.);
    QVERIFY((valArraysEqual(std::valarray<double>{2., 1.}, inputRead)));
}

void TestNeuron::testOr()
{
    Net orNet(2, 1);
    orNet.addHiddenLayer(17);
    orNet.addHiddenLayer(4);
    orNet.connect();
    auto result = orNet.run({1, 0});
    qDebug() << "1 0 -> " << result;
    QVERIFY((std::vector<double>{0.9993292997355148} == result));
    result = orNet.run({0, 0});
    qDebug() << "0 0 -> " << result;
    QVERIFY((std::vector<double>{0.} == result));
    result = orNet.run({0, 1});
    qDebug() << "0 1 -> " << result;
    QVERIFY((std::vector<double>{0.9993292997355148} == result));
}

void TestNeuron::testBackPropagate()
{
    Net backProp(5, 5);
    backProp.addHiddenLayer(5);
    backProp.addHiddenLayer(5);
    backProp.connect();
    for (auto n = 0; n < 350; ++n) {
        backProp.run({1, 2, 3, 4, 5});
        if (n % 100 == 0) {
            qDebug() << backProp.results();
        }
        backProp.backPropagate({.1, .2, .3, .4, .5}, .1);
    }
 }
