#include "TestTrainer.h"
#include "TestListener.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

#include <QDebug>
#include <QImage>

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
            expectation[static_cast<size_t>(c)] = 1.0;
            batch.push_back(expectation);
            if (in.eof()) break;
        }
        result.push_back(batch);
        if (in.eof()) break;
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
            std::vector<double> input;
            for(size_t n = 0; n < 28*28; ++n) {
                char c;
                in.read(&c, 1);
                input.push_back(static_cast<double>(static_cast<unsigned char>(c))/255.);
            }
            batch.push_back(input);
            if (in.eof()) break;
        }
        result.push_back(batch);
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
    auto activateRectifiedLinear = std::make_shared<mimir::helpers::RectifiedLinear>();
    mimir::services::NeuronNet net(2, 1, activateRectifiedLinear);
    net.addHiddenLayer(2, activateRectifiedLinear);
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

void TestTrainer::testTrain()
{
    return;
    mimir::services::NeuronNet net(2, 2);
    net.addHiddenLayer(2);
    net.connect();
    mimir::services::Trainer testee(net);
    testee.addBatch({.05, .1}, {.01, 2});
    auto epochs = testee.run(1500, .000000001, 1);
    qDebug() << net.results();
    QVERIFY(epochs < 1500);
    QVERIFY(testee.currentError() < .000000001);
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
    unsigned int batchSize = 10;
    mimir::services::NeuronNet detector(28*28, 10);
    detector.addHiddenLayer(15);
    detector.connect();
    mimir::services::Trainer trainer(detector);
    auto batches = makeInput(data, batchSize, 500);
    auto expectations = makeExpectations(labels, batchSize, 500);
    auto batch = batches.begin();
    auto expect = expectations.begin();
    qDebug() << "Begin training...";
    unsigned int iterations = 0;
    long itIs = 0;
    long detectedAs = 0;
    std::vector<double> test;
    std::vector<double> expectedResult;
    double eta = .2;// * batch->size();
    while (batch != batches.end()) {
        auto b = (*batch).begin();
        auto e = (*expect).begin();
        while (b != (*batch).end()) {
            trainer.addBatch(*b, *e);
            ++b; ++e;
        }
        trainer.run(iterations == 0 ? 2000 : 100, .0000000001, eta);
        trainer.reset();
        ++iterations;
        qDebug() << "iteration" << iterations << "current error: " << trainer.currentError();
        size_t retestIndex = static_cast<size_t>(std::distance(batches.begin(), batch -1));
        if (retestIndex >= batches.size()) retestIndex = rand()%batches.size();
        test = batches[retestIndex][0];
        expectedResult = expectations[retestIndex][0];
        auto result = detector.run(test);
        itIs = std::distance(expectedResult.begin(), std::max_element(expectedResult.begin(), expectedResult.end()));
        auto maxPtr = std::max_element(result.begin(), result.end());
        detectedAs = std::distance(result.begin(), maxPtr);
        qDebug() << "we've seen a" << itIs << "as a " << detectedAs << "with" << (*maxPtr * 100.) << "% confidence.";
        qDebug() << "detection  " << result << "sum" << std::accumulate(result.begin(), result.end(), 0.);
        qDebug() << "expectation" << expectedResult;
        qDebug() << "eta was" << eta;
 //       eta *= 1.1111111;
        int right = 0, wrong = 0;
        b = batches[iterations].begin();
        e = expectations[iterations].begin();
        std::vector<int> sampleVariance(10);
        std::vector<int> detectVariance(10);
        while (b != batches[iterations].end()) {
            auto res = detector.run(*b);
            long really = std::distance((*e).begin(), std::max_element((*e).begin(), (*e).end()));
            long saw = std::distance(res.begin(), std::max_element(res.begin(), res.end()));
            ++sampleVariance[static_cast<size_t>(really)];
            ++detectVariance[static_cast<size_t>(saw)];
            if (really == saw) {
                ++right;
            } else {
                ++wrong;
            }
            ++b; ++e;
        }
        double ratio = static_cast<double>(right)/static_cast<double>(right + wrong);
        qDebug() << right << " : " << wrong+right << " = correct detect ratio: " << ratio*100. << "%";
        long differentNumsInSample = std::count_if(sampleVariance.begin(), sampleVariance.end(), [](int n) { return  n != 0; });
        long differentNumsInDetect = std::count_if(detectVariance.begin(), detectVariance.end(), [](int n) { return  n != 0; });
        qDebug() << differentNumsInDetect << " different nums seen v " << differentNumsInSample << " different numbers really " << detectVariance << sampleVariance;
//        std::stringstream ss;
//        ss << "iteration_ "<< iterations;
//        saveImage(ss.str(), batches[iterations-1].front(), std::distance(expectations[iterations-1].front().begin(), std::max_element(expectations[iterations-1].front().begin(), expectations[iterations-1].front().end())));
        ++batch; ++expect;
    }
    qDebug() << "finally: error" << trainer.currentError() << "after" << iterations << "batches.";
    QVERIFY(itIs == detectedAs);
}

