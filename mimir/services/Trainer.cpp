#include "Trainer.h"

#include <iostream>
#include <numeric>

#include <boost/range/combine.hpp>

#include "helpers/helpers.h"
#include "helpers/math.h"


namespace mimir {
namespace services {

using models::Matrix;
using namespace helpers::math;

template<typename T>
std::vector<T> avg(std::vector<std::vector<T>> const &v) {
    std::vector<T> values(v.front().size());
    for (auto m : v) {
        values = values + m;
    }
    return values/static_cast<T>(values.size());
}

Trainer::Trainer(NeuronNet &net) :
    _net(net)
{
}

void Trainer::addBatch(std::vector<double> input, std::vector<double> expectation)
{
    if (expectation.size() != _net.outputSize()) {
        throw new std::logic_error("Expectation set size must be equal to the size of the output layer.");
    }
    _batch.push_back({input, expectation});
}

template<typename T>
std::ostream &operator <<(std::ostream & stream, std::vector<T> const &data)
{
    auto precision = std::cout.precision(3);
    stream << "(";
    for(auto v : data) {
        stream << v << ' ';
    }
    stream << ")";
    std::cout.precision(precision);
    return stream;
}

unsigned Trainer::run(size_t batchSize, unsigned maxEpochs, double maxError, double minRate, double eta, MinibatchResultCallback resultCallback)
{
    _callback = resultCallback;
    std::cout.setf(std::ios::fixed, std::ios::floatfield);
    std::cout.precision(10);

    auto batchItem = _batch.begin();
    std::vector<BatchItem> minibatch(batchSize);
    while (batchItem != _batch.end()) {
        auto miniBatchPtr = minibatch.begin();
        for (auto n = 0u; n < batchSize && batchItem != _batch.end(); ++n, ++batchItem) {
            *miniBatchPtr++ = *batchItem;
        }
        auto [epochs, detectRate, currentError] = runMinibatch(minibatch, maxEpochs, maxError, minRate, eta);
        if (resultCallback) {
            resultCallback(currentError, detectRate, epochs);
        }
    }

    return maxEpochs;
}

double Trainer::currentError() const
{
    return _currentError;
}

void Trainer::resetBatches()
{
    _batch.clear();
}

void Trainer::createGradients()
{
    for (auto layer : _net.layers()) {
        if (!layer.isInputLayer()) {
            _biasGradient.push_back(std::vector<double>(layer.size()));
        }
        if (layer.isConnected()) {
            _weightGradient.push_back(Matrix(layer.size(), layer.nextSize()));
        }
    }
}

void Trainer::resetGradients()
{
    if (_biasGradient.size() == 0 || _weightGradient.size() == 0) {
        createGradients();
    }
    for (auto &bias : _biasGradient) {
        bias.assign(bias.size(), 0.);
    }
    for (auto &weight : _weightGradient) {
        weight.fill(0.);
    }
}

void Trainer::backPropagate(const std::vector<std::vector<double> > &results, const std::vector<std::vector<double> > &expectations, double eta)
{
    resetGradients();
    auto result = results.begin();
    auto expect = expectations.begin();
    double gradientWeight = 1./static_cast<double>(results.size());
    while (result != results.end()) {
        auto costDerivative = *expect - *result;
        auto deltaBias = _biasGradient.rbegin();
        auto deltaWeight = _weightGradient.rbegin();
        auto rlayer = _net.layers().rbegin();
        auto delta = - costDerivative * (*rlayer).sigmoidPrime();
        ++rlayer;
        for (; rlayer != _net.layers().rend(); ++rlayer) {
            if ((*rlayer).isConnected()) {
                *deltaWeight = (Matrix(delta) * helpers::toArray((*rlayer).hypothesis())).transpose() * gradientWeight;
                ++deltaWeight;
            }
            if (!(*rlayer).isInputLayer()) {
                *deltaBias = delta * gradientWeight;
                delta = ((*rlayer).weights() * delta).column(0) * (*rlayer).sigmoidPrime();
                ++deltaBias;
            }
        }
        ++result; ++expect;
    }
    double weightDecay = .000;//5;
    auto deltaB = _biasGradient.begin();
    auto deltaW = _weightGradient.begin();
    for (auto &l : _net.layers()) {
        if (!l.isInputLayer()) {
            l.setBiases(l.biases() - eta * *deltaB);
            ++deltaB;
        }
        if (l.isConnected()) {
            l.setWeights(l.weights() - eta * (*deltaW + weightDecay * l.weights()));
            ++deltaW;
        }
    }
}

std::tuple<unsigned, double, double> Trainer::runMinibatch(const std::vector<Trainer::BatchItem> &miniBatch, unsigned maxEpochs, double maxError, double minRate, double eta)
{
    auto epoch = 0u;
    double error = 0.;
    double rate = 0.;
    while (epoch < maxEpochs) {
        ++epoch;
        std::vector<std::vector<double>> results;
        std::vector<std::vector<double>> expectations;
        for (auto item : miniBatch) {
            auto result = _net.run(item.input);
            results.push_back(result);
            expectations.push_back(item.expectation);
        }
        error += mse(results, expectations);
        auto currentDetectRate = detectRate(results, expectations);
        rate += currentDetectRate;

        if (error <= maxError && currentDetectRate >= minRate) {
            break;
        }
        backPropagate(results, expectations, eta);
    }
    return {epoch, rate/static_cast<double>(epoch), error/static_cast<double>(epoch)};
}

double Trainer::mse(const std::vector<std::vector<double> > &results, const std::vector<std::vector<double>> &expectations) const
{
    auto r = results.begin();
    auto e = expectations.begin();
    double mse = 0;
    while (r != results.end()) {
        auto diff = *r - *e;
        auto error = std::accumulate(diff.begin(), diff.end(), 0.)/static_cast<double>(diff.size());
        mse += error*error;
        ++r; ++e;
    }
    return mse / static_cast<double>(results.size());
}

double Trainer::detectRate(const std::vector<std::vector<double> > &results, const std::vector<std::vector<double> > &expectations)
{
    double correct = 0.;
    double incorrect = 0.;
    for (auto tuple : boost::range::combine(results, expectations)) {
        auto seen = std::distance(tuple.get<0>().begin(), std::max_element(tuple.get<0>().begin(), tuple.get<0>().end()));
        auto really = std::distance(tuple.get<1>().begin(), std::max_element(tuple.get<1>().begin(), tuple.get<1>().end()));
        if (seen == really) {
            correct += 1.;
        } else {
            incorrect += 1.;
        }
    }
    return correct/(correct+incorrect);
}

} // namespace services
} // namespace mimir
