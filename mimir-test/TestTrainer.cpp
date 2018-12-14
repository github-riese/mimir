#include "TestTrainer.h"
#include "TestListener.h"

#include <fstream>
#include <iostream>

#include <QDebug>

#include <services/Trainer.h>

REGISTER_TEST(TestTrainer)

std::vector<std::vector<std::vector<double>>> makeExpectations(std::ifstream &in, unsigned batchSize, unsigned batchCount)
{
    std::vector<std::vector<std::vector<double>>> result;
    for (unsigned batches = 0; batches < batchCount; ++batches) {
        std::vector<std::vector<double>> batch;
        for(unsigned item = 0; item < batchSize; ++item) {
            char c;
            in.read(&c, 1);
            std::vector<double> expectation(10, 0);
            expectation[static_cast<unsigned char>(c)] = 1.0;
            batch.push_back(expectation);
        }
        result.push_back(batch);
    }
    return result;
}

std::vector<std::vector<std::vector<double>>> makeInput(std::ifstream &in, unsigned batchSize, unsigned batchCount)
{
    std::vector<std::vector<std::vector<double>>> result;
    for (unsigned batches = 0; batches < batchCount; ++batches) {
        std::vector<std::vector<double>> batch;
        for(unsigned item = 0; item < batchSize; ++item) {
            std::istreambuf_iterator<char> reader(in);
            std::vector<double> expectation;
            for(size_t n = 0; n < 28*28; ++n) {
                char c;
                in.read(&c, 1);
                expectation.push_back(static_cast<double>(static_cast<unsigned char>(c))/ 255.);
            }
            batch.push_back(expectation);
        }
        result.push_back(batch);
    }
    return result;
}

TestTrainer::TestTrainer(QObject *parent) : QObject(parent)
{

}

void TestTrainer::testTrain()
{
    mimir::services::NeuronNet net(5, 5);
    net.connect();
    mimir::services::Trainer testee(net);
    testee.addBatch({1, 2, 3, 4, 5}, {.5, .4, .3, .2, .1});
    auto epochs = testee.run(1500, .000000001, .9);
    qDebug() << net.results();
    QVERIFY(epochs < 1500);
    QVERIFY(testee.currentError() < .000000001);
}

void TestTrainer::testImageDetect()
{
    std::ifstream labels("/Users/riese/c-stuff/mimir/mimir-test/assets/train-labels-idx1-ubyte");
    std::ifstream data("/Users/riese/c-stuff/mimir/mimir-test/assets/train-images-idx3-ubyte");
    char x[32] = {};
    labels.read(x, 8);
    data.read(x, 16);
    mimir::services::NeuronNet detector(28*28, 10);
//    detector.addHiddenLayer(28*14);
//    detector.addHiddenLayer(14*14);
    detector.addHiddenLayer(15);
    detector.connect();
    mimir::services::Trainer trainer(detector);
    auto batches = makeInput(data, 100, 40);
    auto expectations = makeExpectations(labels, 100, 40);
    auto batch = batches.begin();
    auto expect = expectations.begin();
    qDebug() << "Begin training...";
    unsigned int iterations = 0;
    long itIs = 0;
    long detectedAs = 0;
    std::vector<double> test;
    std::vector<double> expectedResult;
    while (batch != batches.end()) {
        auto b = (*batch).begin();
        auto e = (*expect).begin();
        while (b != (*batch).end()) {
            trainer.addBatch(*b, *e);
            ++b; ++e;
        }
        trainer.run(10000, .000000001, 100);
        trainer.reset();
        ++iterations;
        qDebug() << "iteration" << iterations << "current error: " << trainer.currentError();
        ++batch; ++expect;
        test = batches[0][iterations];
        expectedResult = expectations[0][iterations];
        auto result = detector.run(test);
        itIs = std::distance(expectedResult.begin(), std::max_element(expectedResult.begin(), expectedResult.end()));
        auto maxPtr = std::max_element(result.begin(), result.end());
        detectedAs = std::distance(result.begin(), maxPtr);
        qDebug() << "we've seen a" << itIs << "as a " << detectedAs << "with" << (*maxPtr * 100.) << "% certainty.";
        qDebug() << "detection" << result;
    }
    qDebug() << "finally: error" << trainer.currentError() << "after" << iterations << "batches.";
    QVERIFY(itIs == detectedAs);
}

