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

NeuronNet::NeuronNet()
{
}

NeuronNet::NeuronNet(long inputs, long outputs, helpers::Activation* outputActivation)
{
    Layer input(nullptr);
    for (auto n = 0; n < inputs; ++n) {
        input.addNeuron(0);
    }
    input.setIsInput(true);
    _layers.push_back(input);
    Layer output(outputActivation);
    for (auto n = 0; n < outputs; ++n) {
        output.addNeuron(static_cast<double>(rand()%100)/10000.);
    }
    _layers.push_back(output);
}

void NeuronNet::addHiddenLayer(size_t numNeurons, helpers::Activation* activation)
{
    if (_layers[0].isConnected()) {
        throw std::logic_error("can't add layers after net is connected.");
    }
    Layer l(activation == nullptr ? _layers.back().activation() : activation);
    for (auto n = 0u; n < numNeurons; ++n) {
        l.addNeuron(static_cast<double>(rand()%100)/1000.);
    }
    _layers.insert(_layers.end() -1, l);
}

void NeuronNet::appendLayer(size_t numNeurons, helpers::Activation *activation)
{
    Layer l(activation == nullptr && _layers.size() > 0 ? _layers.back().activation() : activation);
    for (auto n = 0u; n < numNeurons; ++n) {
        l.addNeuron(static_cast<double>(rand()%100)/1000.);
    }
    _layers.push_back(l);
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

size_t NeuronNet::inputSize() const
{
    return _layers.front().size();
}

size_t NeuronNet::outputSize() const
{
    return _layers.back().size();
}

size_t NeuronNet::numberOfLayers() const
{
    return _layers.size();
}

void NeuronNet::setBias(size_t layer, size_t neuron, double value)
{
    _layers[layer].setBias(neuron, value);
}

void NeuronNet::setBiases(size_t layer, const std::vector<double> &biases)
{
    _layers[layer].setBiases(biases);
}

void NeuronNet::setWeight(size_t layer, size_t neuron, size_t nextLayerNeuron, double value)
{
    _layers[layer].setWeight(neuron, nextLayerNeuron, value);
}

void NeuronNet::setWeigths(size_t layer, const models::Matrix &weights)
{
    _layers[layer].setWeights(weights);
}

std::string NeuronNet::name() const
{
    return _name;
}

void NeuronNet::setName(const std::string &name)
{
    _name = name;
}

}
}
