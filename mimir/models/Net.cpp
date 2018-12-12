#include "Net.h"

#include <algorithm>
#include <iterator>

#include "helpers/math.h"
#include "helpers/helpers.h"

namespace mimir {
namespace models {

Net::Net(long inputs, long outputs) :
    _output()
{
    Layer input;
    for (auto n = 0; n < inputs; ++n) {
        input.addNeuron({});
    }
    _layers.push_back(input);
    for (auto n = 0; n < outputs; ++n) {
        _output.addNeuron({});
    }

}

void Net::addHiddenLayer(int numNeurons)
{
    if (_layers[0].isConnected()) {
        throw std::logic_error("can't add layers after net is connected.");
    }
    Layer l;
    for (auto n = 0; n < numNeurons; ++n) {
        l.addNeuron({});
    }
    _layers.push_back(l);
}

void Net::connect()
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
    (*previous).connect(_output);
}

std::vector<double> Net::run(std::vector<double> inputs)
{
    Layer &input = _layers.front();
    input.reset();
    input.addInput(inputs);
    for (auto &layer : _layers) {
        layer.run();
    }
    return results();
}


std::vector<double> Net::results()
{
    return _output.values();
}

/**
 * @brief Net::backPropagate
 * will back propagate new biases & weights in order to achive output as defined in expectation
 * @param expectation
 */
void Net::backPropagate(const std::valarray<double> &expectation, double eta)
{
    std::valarray<double> delta = helpers::toArray(results()) - expectation;
    auto deltaV = helpers::toVector(delta);
    _output.changeBiases(delta * _output.deriviateActivations() * eta);
    _layers.back().changeWeights(_layers.back().weights().transposed() * helpers::toVector((delta * eta)));
    auto layer = _layers.rbegin();
    auto previousLayer = layer + 1;
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
