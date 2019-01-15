#include "NeuronNet.h"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <sstream>
#include <random>

#include "helpers/math.h"
#include "helpers/helpers.h"
#include "helpers/activations.h"
#include "ActivationsManager.h"

using mimir::models::Layer;
using mimir::models::Matrix;
using namespace mimir::helpers::math;

namespace mimir {
namespace services {

NeuronNet::NeuronNet()
{
}

NeuronNet::NeuronNet(size_t inputs, size_t outputs, std::string const &outputActivation)
{
    appendLayer(inputs);
    _layers.front().setIsInput(true);
    appendLayer(outputs, outputActivation);
}

void NeuronNet::addHiddenLayer(size_t numNeurons, std::string const &activation)
{
    if (_layers[0].isConnected()) {
        throw std::logic_error("can't add layers after net is connected.");
    }
    Layer l(activation.empty() ? _layers.back().activation() : getActivationsManager().get(activation));
    for (auto n = 0u; n < numNeurons; ++n) {
        l.addNode(static_cast<double>(rand()%200)/10000. - .001);
    }
    _layers.insert(_layers.end() -1, l);
}

void NeuronNet::appendLayer(size_t numNeurons, const std::string &activation)
{
    std::random_device randDev;
    std::mt19937 twister(randDev());
    std::normal_distribution<double> random(-.001, .001);
    Layer l(activation.empty() && _layers.size() > 0 ? _layers.back().activation() : getActivationsManager().get(activation));
    for (auto n = 0u; n < numNeurons; ++n) {
        l.addNode(random(randDev));
    }
    l.setIsOutputLayer(true);
    if (!_layers.empty()) {
        _layers.back().setIsOutputLayer(false);
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
    (*previous).connect(Layer());
}

std::vector<double> NeuronNet::run(std::vector<double> inputs)
{
    Layer &input = _layers.front();
    input.setInput(inputs);
    auto layer = _layers.begin();
    size_t n = _layers.size() -1;
    while (n-- > 0) {
        auto nextInput = layer->run();
        ++layer;
        layer->setInput(nextInput);
    }
    return results();
}


std::vector<double> NeuronNet::results()
{
    return _layers.back().hypothesis();
}

size_t NeuronNet::sizeOfLayer(size_t layer) const
{
    if (layer != -1u && layer >= _layers.size()) {
        return -1u;
    }
    if (layer == -1u) {
        return _layers.back().size();
    }
    return _layers.at(layer).size();
}

size_t NeuronNet::numberOfLayers() const
{
    return _layers.size();
}

bool NeuronNet::addNode(size_t layer, double bias, std::vector<double> weightsIn, std::valarray<double> weightsOut)
{
    if (layer >= _layers.size() && layer != -1u) {
        return false;
    }
    layer = layer == -1u ? _layers.size() - 1 : layer;
    auto &modifiedLayer = _layers[layer];
    modifiedLayer.addNode(bias, weightsOut);
    if (modifiedLayer.isConnected() && !modifiedLayer.isInputLayer()) {
        return _layers[layer - 1].reconnect(modifiedLayer, weightsIn);
    }
    return true;
}

const models::Layer &NeuronNet::layer(size_t n) const
{
    if (n >= _layers.size() && n != -1u) {
        std::stringstream m;
        m << "Can't give layer " << n << " as this net has only " << _layers.size() << "layers.";
        throw std::out_of_range(m.str());
    }
    if (n == -1u) {
        return _layers.back();
    }
    return _layers.at(n);
}

models::Layer &NeuronNet::layer(size_t n)
{
    if (n >= _layers.size() && n != -1u) {
        std::stringstream m;
        m << "Can't give layer " << n << " as this net has only " << _layers.size() << "layers.";
        throw std::out_of_range(m.str());
    }
    if (n == -1u) {
        return _layers.back();
    }
    return _layers.at(n);
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
