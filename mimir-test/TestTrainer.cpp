#include "TestTrainer.h"
#include "TestListener.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

#include <QDebug>
#include <QImage>

#include <services/Trainer.h>
#include <services/NeuronNetSerializer.h>
#include <models/activation/Softmax.h>

REGISTER_TEST(TestTrainer)

std::vector<std::vector<double>> makeExpectations(std::ifstream &in, unsigned maxItems)
{
    std::vector<std::vector<double>> result;
    while (maxItems-- > 0) {
        char c;
        in.read(&c, 1);
        std::vector<double> expectation(10, 0);
        expectation[static_cast<size_t>(c)] = 1.0;
        result.push_back(expectation);
        if (in.eof()) break;
    }
    return result;
}

std::vector<std::vector<double>> makeInput(std::ifstream &in, unsigned maxItems)
{
    std::vector<std::vector<double>> result;
    while (maxItems-- > 0) {
    std::istreambuf_iterator<char> reader(in);
    std::vector<double> input;
    for(size_t n = 0; n < 28*28; ++n) {
        char c;
        in.read(&c, 1);
        input.push_back(static_cast<double>(static_cast<unsigned char>(c))/255.);
    }
    result.push_back(input);
    if (in.eof()) break;
    }
    return result;
}

TestTrainer::TestTrainer(QObject *parent) : QObject(parent)
{

}

void TestTrainer::testXOR()
{
    srand(static_cast<unsigned>(time(nullptr)));
    mimir::services::NeuronNet net(2, 1, "rectifiedLinear");
    net.addHiddenLayer(2, "rectifiedLinear");
    net.connect();

    net.setWeigths(0, mimir::models::Matrix({
                                                {1, 1},
                                                {1, 1}
                                            }));
    net.setWeigths(1, mimir::models::Matrix(std::vector<std::valarray<double>>{
                                                {1},
                                                {-2}
                                            }));

    net.setBias(1, 0, 0);
    net.setBias(1, 1, -1);

    net.setBias(2, 0, 0);

    /*mimir::services::Trainer trainer(net);
    trainer.addBatch({0, 0}, {0});
    trainer.addBatch({0, 1}, {1});
    trainer.addBatch({1, 0}, {1});
    trainer.addBatch({1, 1}, {0});
    auto epochs = trainer.run(1000, .00000001, 1);*/
    qDebug() << "0 xor 0" << net.run({0, 0});
    qDebug() << "0 xor 1" << net.run({0, 1});
    qDebug() << "1 xor 0" << net.run({1, 0});
    qDebug() << "1 xor 1" << net.run({1, 1});
    //QVERIFY(epochs < 1000);
}

void TestTrainer::testChangeNet()
{
    mimir::services::NeuronNet net(4, 1, "sigmoid");
    net.addNode(1);
    net.connect();
    net.addNode(0, {}, {1, 2});
    net.addNode(1, -5, {0, 0, 0, 0, 0});
    net.addNode(1, -5);
    net.addNode(1, -5, {.001});
    QVERIFY(net.sizeOfLayer(0) == 5);
    QVERIFY(net.sizeOfLayer(1) == 5);
    QVERIFY(net.layers().front().weights().column(4) == std::vector<double>(5, .001));
    auto result = net.run({1, 2, 3, 4, 5});
    QVERIFY(result.size() == 5u);
    // net had been initialized with near null random values on connect.
    // that should result in values of roundabout .5
    // the nodes added after connecting had been set to a small bias
    // that should pull the result in the direction of zero
    std::vector<double> expectation = {.5, .5, 0, 0, 0};
    auto res = result.begin();
    auto exp = expectation.begin();
    while (res != result.end()) {
        // as we deal random values we allow for a great fuzzyness here.
        // just check the general direction.
        QVERIFY(std::abs(*res - *exp) < .01);
        ++res; ++exp;
    }
}

void TestTrainer::testSoftMax()
{
    mimir::models::activation::Softmax softMax;
    std::vector<double> data{1000, 2000, 3000};
    softMax.activate(data);
    QVERIFY((data == std::vector<double>{0, 0, 1}));
}

void TestTrainer::testTrain()
{
    return;
    mimir::services::NeuronNet net(2, 2, "softmax");
    net.addHiddenLayer(2);
    net.connect();
    mimir::services::Trainer testee(net);
    testee.addTrainingData({.05, .1}, {.0, 1.});
    auto epochs = testee.run(1, 1500, .003, .8, .1);
    qDebug() << net.results();
    qDebug() << testee.currentError();
    qDebug() << epochs;
    QVERIFY(epochs < 1500);
//    QVERIFY(testee.currentError() < .003);
}

void saveImage(std::string const &name, std::vector<double> const &pixels, int expectedNumber) {

    QImage img({28, 28}, QImage::Format::Format_Grayscale8);
    std::vector<uchar> imgdata;
    for(double grey : pixels)
        imgdata.push_back(static_cast<uchar>(255.*grey));
    memcpy(img.bits(),imgdata.data(), 28*28);
    QString fileName(QString::fromStdString(name));
    fileName += QStringLiteral("_%1.png").arg(expectedNumber);
    img.save(fileName);
}

void TestTrainer::testImageDetect()
{
    //return;
    std::ifstream labels("/Users/riese/c-stuff/mimir/mimir-test/assets/train-labels-idx1-ubyte");
    std::ifstream data("/Users/riese/c-stuff/mimir/mimir-test/assets/train-images-idx3-ubyte");
    char x[32] = {};
    labels.read(x, 8);
    data.read(x, 16);
    unsigned int batchSize = 5;
    mimir::services::NeuronNet detector(28*28, 10, "softmax");
    detector.addHiddenLayer(9, "rectifiedLinear");
    detector.connect();
    mimir::services::Trainer trainer(detector);
    trainer.createGradients();
    auto batches = makeInput(data, 250);
    auto expectations = makeExpectations(labels, 250);
    auto batch = batches.begin();
    auto expect = expectations.begin();
    qDebug() << "Begin training...";
    while (batch != batches.end() && expect != expectations.end()) {
        trainer.addTrainingData(*batch++, *expect++);
    }
    double learningRate = .2;
    int minibatch = 0;
    auto batchResult = [&minibatch, &batches, &expectations, &detector, &batchSize](double currentError, double detectRate, unsigned epochsNeeded) {
        qDebug() << "minibatch" << minibatch++ << "error" << currentError << "detected" << detectRate*100. << "%" << "epochs: "<< epochsNeeded;
        if (detectRate > .8) {
            auto result = detector.run(*(batches.begin() + static_cast<unsigned>(minibatch)*batchSize-1));
            qDebug() << result << *(expectations.begin() + static_cast<unsigned>(minibatch)*batchSize-1);
        }
    };
    trainer.run(batchSize, 1000, 1e-5, .9, learningRate, batchResult);
    int right = 0, wrong = 0;
    for (auto n = 0u; n < batches.size(); n += 5) {
        auto seen = detector.run(batches.at(n));
        auto expected = expectations.at(n);
        auto ds = std::distance(seen.begin(), std::max_element(seen.begin(), seen.end()));
        auto de = std::distance(expected.begin(), std::max_element(expected.begin(), expected.end()));
        qDebug() << "seen:" << ds
                 << "expected:" << de;
        if (ds == de) ++right;
        else ++wrong;
    }
    qDebug() << "right" << right;
    qDebug() << "wrong" << wrong;
/*    minibatch = 0;
    batchSize /= 2;
    trainer.run(batchSize, 100, .000001, .97, eta/2., batchResult);*/
    mimir::services::NeuronNetSerializer serializer;
    std::ofstream file("mnist_detect.mimir");
    serializer.serialize(detector, file);
}

