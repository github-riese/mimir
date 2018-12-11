#include "Net.h"

#include <algorithm>
#include <iterator>

#include "helpers/math.h"

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

std::valarray<double> Net::run(std::vector<double> inputs)
{
    Layer &input = _layers.front();
    input.reset();
    input.addInput(inputs);
    for (auto &layer : _layers) {
        layer.run();
    }
    return results();
}


std::valarray<double> Net::results()
{
    return _output.values();
}

/**
 * @brief Net::backPropagate
 * will back propagate new biases & weights in order to achive output as defined in expectation
 * @param expectation
 */
void Net::backPropagate(const std::valarray<double> &expectation)
{
    std::valarray<double> deltaLPlusOne = (_output.values() - expectation) * _output.deriviateActivations();
    _output.setBiases(deltaLPlusOne);
    Layer *lPlusOne = &_output;
    Layer *l;
    auto hiddenLayer = _layers.rbegin();
    while (hiddenLayer != _layers.rend()) {
        l = &(*hiddenLayer);
        ++hiddenLayer;
        std::valarray<double> sigmaDerivateZ = l->deriviateActivations();
        std::vector<std::valarray<double>> weights = l->weights();
        mimir::helpers::math::transposeMatrix(weights);
        lPlusOne = l;
    }
    l = &_layers.front();
}

}
}
