#include "NeuronNet.h"

#include <algorithm>
#include <iterator>

#include "helpers/math.h"
#include "helpers/helpers.h"

using mimir::models::Layer;
using mimir::models::Matrix;

namespace mimir {
namespace services {

NeuronNet::NeuronNet(long inputs, long outputs) :
    _output()
{
    Layer input;
    for (auto n = 0; n < inputs; ++n) {
        input.addNeuron({});
    }
    _layers.push_back(input);
    Layer output;
    for (auto n = 0; n < outputs; ++n) {
        output.addNeuron({});
    }
    _layers.push_back(output);
}

void NeuronNet::addHiddenLayer(int numNeurons)
{
    if (_layers[0].isConnected()) {
        throw std::logic_error("can't add layers after net is connected.");
    }
    Layer l;
    for (auto n = 0; n < numNeurons; ++n) {
        l.addNeuron({});
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
    input.reset();
    input.addInput(inputs);
    auto layer = _layers.begin();
    auto output = _layers.end() -1;
    while (layer != output) {
        layer->run();
        ++layer;
    }
    return results();
}


std::vector<double> NeuronNet::results()
{
    return _layers.back().values();
}

/**
 * @brief Net::backPropagate
 * will back propagate new biases & weights in order to achive output as defined in expectation
 * @param expectation
 */
void NeuronNet::backPropagate(const std::valarray<double> &expectation, double eta)
{
    std::valarray<double> delta = helpers::toArray(results()) - expectation;
    auto deltaV = helpers::toVector(delta);
    auto layer = _layers.rbegin();
    auto previousLayer = layer + 1;
    layer->changeBiases(delta * layer->deriviateActivations() * eta);
    previousLayer->changeWeights(previousLayer->weights().transposed() * helpers::toVector((delta * eta)));
    ++layer; ++previousLayer;
    while (previousLayer != _layers.rend()) {
        auto sigmoidPrime = (*layer).deriviateActivations();
        Matrix t = layer->weights();
        t *= deltaV;
        deltaV = t.column(0);
        delta = helpers::toArray(deltaV) * eta;
        (*layer).changeBiases(delta);
        t = previousLayer->values();
        t *= delta;
        previousLayer->changeWeights(t);
        ++layer; ++previousLayer;
    }
}

}
}
