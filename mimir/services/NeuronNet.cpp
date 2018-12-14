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
void NeuronNet::backPropagate(const std::vector<double> &costDerivative, double eta)
{
    auto cost = helpers::toArray(costDerivative);
    std::vector<double> costV = costDerivative;

    std::vector<std::vector<double>> nablaBiases;
    std::vector<std::vector<double>> deltaNablaBiases;
    std::vector<Matrix> deltaNablaWeights;
    std::vector<Matrix> nablaWeights;
    std::vector<std::vector<double>> sigmoidPrimes;

    std::vector<std::vector<double>> biases;
    std::vector<Matrix> weights;

    std::vector<std::vector<double>> activations;

    for (auto layer : _layers) {
        nablaBiases.push_back(std::vector<double>(layer.size()));
        deltaNablaBiases.push_back(std::vector<double>(layer.size()));
        activations.push_back(layer.values());
        if (layer.isConnected()) {
            nablaWeights.push_back(Matrix(layer.size(), layer.nextSize() == 0 ? 1 : layer.nextSize(), 0));
            deltaNablaWeights.push_back(Matrix(layer.size(), layer.nextSize() == 0 ? 1 : layer.nextSize(), 0));
        }
        sigmoidPrimes.push_back(helpers::toVector(layer.sigmoidPrime()));
    }

    auto layer = _layers.rbegin();
    auto previousLayer = layer + 1;
    auto deltaNablaBias = deltaNablaBiases.rbegin();
    auto deltaNablaWeight = deltaNablaWeights.rbegin();
    auto activation = activations.rbegin();
    auto sigmoidPrime = sigmoidPrimes.rbegin();

    std::vector<double> delta = costDerivative;
    *deltaNablaBias = costDerivative;
    *deltaNablaWeight = Matrix(delta) * helpers::toArray(*activation);
    ++deltaNablaBias; ++deltaNablaWeight; ++activation; ++sigmoidPrime; ++previousLayer;
    while (deltaNablaBias != deltaNablaBiases.rend()) {
        delta = helpers::toVector(helpers::toArray(((*previousLayer).weights().transposed() * delta).column(0)) * helpers::toArray(*sigmoidPrime));
        *deltaNablaBias = delta;
        if (deltaNablaWeight != deltaNablaWeights.rend()) {
            *deltaNablaWeight = Matrix(delta) * helpers::toArray(*(activation + 1));
        }
        ++deltaNablaBias; ++deltaNablaWeight; ++activation; ++sigmoidPrime; ++previousLayer;
    }
}

size_t NeuronNet::outputSize() const
{
    return _layers.back().size();
}

}
}
