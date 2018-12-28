#include "NeuronNet.h"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <cstdlib>

#include "helpers/math.h"
#include "helpers/helpers.h"
#include "helpers/activations.h"

using mimir::models::Layer;
using mimir::models::Matrix;
using namespace mimir::helpers::math;

namespace mimir {
namespace services {

NeuronNet::NeuronNet(long inputs, long outputs, std::shared_ptr<helpers::Activation> activation)
{
    Layer input(nullptr);
    for (auto n = 0; n < inputs; ++n) {
        input.addNeuron(0);
    }
    input.setIsInput(true);
    _layers.push_back(input);
    Layer output(activation);
    for (auto n = 0; n < outputs; ++n) {
        output.addNeuron(static_cast<double>(rand()%100)/10000.);
    }
    _layers.push_back(output);
}

void NeuronNet::addHiddenLayer(int numNeurons, std::shared_ptr<helpers::Activation> activation)
{
    if (_layers[0].isConnected()) {
        throw std::logic_error("can't add layers after net is connected.");
    }
    Layer l(activation);
    for (auto n = 0; n < numNeurons; ++n) {
        l.addNeuron(static_cast<double>(rand()%100)/1000.);
    }
    _layers.insert(_layers.end() -1, l);
}

void NeuronNet::connect()
{
    if (_layers[0].isConnected()) {
        return;
    }
    auto previous = _layers.begin();
    auto next = previous;
    ++next;
    while (next != _layers.end()) {
        (*previous).connect(*(next));
        ++previous; ++next;
    }
}

std::vector<double> NeuronNet::run(std::vector<double> inputs)
{
    Layer &input = _layers.front();
    input.setInput(inputs);
    auto layer = _layers.begin();
    size_t n = _layers.size() -1;
    while (n-- > 0) {
        layer->run();
        ++layer;
    }
    return results();
}


std::vector<double> NeuronNet::results()
{
    return _layers.back().hypothesis();
}

/**
 * @brief Net::backPropagate
 * will back propagate new biases & weights in order to achive output as defined in expectation
 * @param expectation
 */
void NeuronNet::backPropagate(const std::vector<std::vector<double>> &results, std::vector<std::vector<double>> const &expectations, double eta)
{
    std::vector<std::vector<double>> biasChanges;
    std::vector<Matrix> weightChanges;
    double batchSize = static_cast<double>(results.size());
    double oneByBatchSize = 1./batchSize;
    double lambda = 0.1;

    for (auto l : _layers) {
        if (!l.isInputLayer()) {
            biasChanges.push_back(std::vector<double>(l.size(), 0.));
        }
        if (l.isConnected()) {
            weightChanges.push_back(Matrix(l.size(), l.nextSize(), 0.));
        }
    }
    auto resultItem = results.begin();
    auto expectItem = expectations.begin();
    for(;resultItem != results.end(); ++expectItem, ++resultItem) {
        auto [nablaBias, nablaWeight] = deltaNabla(*expectItem - *resultItem);
        biasChanges += nablaBias;
        weightChanges += nablaWeight;
    }
    auto deltaW = weightChanges.begin();
    auto deltaB = biasChanges.begin();
    for (auto &l : _layers) {
        if (!l.isInputLayer()) {
            l.setBiases(l.biases() - eta * (oneByBatchSize * *deltaB));
            ++deltaB;
        }
        if (l.isConnected()) {
            l.setWeights(l.weights() - eta * ((oneByBatchSize * *deltaW) + lambda * l.weights()));
            ++deltaW;
        }
    }
}

std::tuple<std::vector<std::vector<double> >, std::vector<models::Matrix> >
  NeuronNet::deltaNabla(std::vector<double> const &costDerivative)
{
    std::vector<std::vector<double>> biasChanges;
    std::vector<Matrix> weightChanges;
    for (auto l : _layers) {
        if (!l.isInputLayer()) {
            biasChanges.push_back(std::vector<double>(l.size(), .0));
        }
        if (l.isConnected()) {
            weightChanges.push_back(Matrix(l.size(), l.nextSize(), .0));
        }
    }
    auto deltaBias = biasChanges.rbegin();
    auto deltaWeight = weightChanges.rbegin();
    auto rlayer = _layers.rbegin();
    auto delta = - costDerivative * (*rlayer).sigmoidPrime();
    ++rlayer;
    for (; rlayer != _layers.rend(); ++rlayer) {
        if ((*rlayer).isConnected()) {
            *deltaWeight = (Matrix(delta) * helpers::toArray((*rlayer).hypothesis())).transpose();
            ++deltaWeight;
        }
        if (!(*rlayer).isInputLayer()) {
            *deltaBias = delta;
            delta = ((*rlayer).weights() * delta).column(0) * (*rlayer).sigmoidPrime();
            ++deltaBias;
        }
    }
    return {biasChanges, weightChanges};
}

size_t NeuronNet::outputSize() const
{
    return _layers.back().size();
}

void NeuronNet::setBias(size_t layer, size_t neuron, double value)
{
    _layers[layer].setBias(neuron, value);
}

void NeuronNet::setWeight(size_t layer, size_t neuron, size_t nextLayerNeuron, double value)
{
    _layers[layer].setWeight(neuron, nextLayerNeuron, value);
}

void NeuronNet::setWeigths(size_t layer, const models::Matrix &weights)
{
    _layers[layer].setWeights(weights);
}

std::vector<models::Matrix> NeuronNet::addMatices(const std::vector<models::Matrix> &left, const std::vector<models::Matrix> &right) const
{
    std::vector<Matrix> result;
    auto l = left.begin();
    auto r = right.begin();
    while (l != left.end()) {
        result.push_back({*l + r->value(0,0)});
        ++l; ++r;
    }
    return result;
}

std::vector<std::vector<double> > NeuronNet::addVectors(const std::vector<std::vector<double> > &left, const std::vector<std::vector<double> > &right) const
{
    std::vector<std::vector<double>> result;
    auto l = left.begin();
    auto r = right.begin();
    while (l != left.end()) {
        std::vector<double> t(l->size());
        std::transform(l->begin(), l->end(), r->begin(), t.begin(), [](auto left, auto right)->auto { return left + right; });
        result.push_back(t);
        ++l; ++r;
    }
    return result;
}

}
}
