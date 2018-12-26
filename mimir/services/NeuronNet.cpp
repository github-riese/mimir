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
        l.addNeuron(static_cast<double>(rand()%100)/-1000.);
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
    return _layers.back().values();
}

/**
 * @brief Net::backPropagate
 * will back propagate new biases & weights in order to achive output as defined in expectation
 * @param expectation
 */
void NeuronNet::backPropagate(const std::vector<std::vector<double>> &results, std::vector<std::vector<double>> const &expectations, double eta)
{
    std::vector<std::vector<double>> deltaBiases;
    std::vector<Matrix> deltaWeights;

    std::vector<std::vector<double>> deltaNablaBiases;
    std::vector<Matrix> deltaNablaWeights;

    std::vector<std::vector<double>> biases;
    std::vector<Matrix> weights;

    double oneByM = 1./static_cast<double>(results.size());
    double lambda = 1.;

    for (auto layer : _layers) {
        if (!layer.isInputLayer()) {
            deltaBiases.push_back(std::vector<double>(layer.size(), 1.));
        }
        if (layer.isConnected()) {
            deltaWeights.push_back(Matrix(layer.size(), layer.nextSize() == 0 ? 1 : layer.nextSize(), .1));
        }
    }

    auto x = results.begin();
    auto y = expectations.begin();
    while (x != results.end()) {
        auto diff = *x - *y;
        auto deltas = deltaNabla(diff);
        deltaNablaBiases = std::get<0>(deltas);
        deltaNablaWeights = std::get<1>(deltas);
        deltaBiases = addVectors(deltaBiases, deltaNablaBiases);
        deltaWeights = addMatices(deltaWeights, deltaNablaWeights);

        ++x; ++y;
    }

    eta = eta * oneByM;

    auto deltaWeight = deltaWeights.begin();
    auto deltaBias = deltaBiases.begin();
    for (auto &layer : _layers) {
        if (!layer.isInputLayer()) {
            layer.setBiases(layer.biases() - *deltaBias  * eta);
            ++deltaBias;
        }
        if (layer.isConnected()) {
            auto applyDeltaWeight = *deltaWeight;
            applyDeltaWeight *= oneByM;
            auto currentWeight = layer.weights();
            currentWeight *= lambda;
            layer.setWeights(layer.weights() - (applyDeltaWeight + currentWeight)*eta);
            ++deltaWeight;
        }
    }

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

std::tuple<std::vector<std::vector<double> >, std::vector<models::Matrix> >
  NeuronNet::deltaNabla(std::vector<double> const &costDerivative) const
{
    std::vector<std::vector<double>> deltaNablaBiases;
    std::vector<Matrix> deltaNablaWeights;
    std::vector<std::vector<double>> activations;

    for (auto layer : _layers) {
        if (!layer.isInputLayer()) {
            deltaNablaBiases.push_back(std::vector<double>(layer.size(), 0));
        }
        if (layer.isConnected()) {
            deltaNablaWeights.push_back(Matrix(layer.size(), layer.nextSize() == 0 ? 1 : layer.nextSize(), 0.));
        }
        activations.push_back(layer.values());
    }

    auto deltaNablaBias = deltaNablaBiases.rbegin();
    auto deltaNablaWeight = deltaNablaWeights.rbegin();
    auto activation = activations.rbegin();

    auto layer = _layers.rbegin();

    auto delta = costDerivative * layer->sigmoidPrime();
    *deltaNablaBias = delta;
    *deltaNablaWeight = (Matrix((delta)) * helpers::toArray(*(activation )));
    ++deltaNablaBias; ++deltaNablaWeight; ++activation;
    ++layer;
    while (deltaNablaBias != deltaNablaBiases.rend()) {
        auto weightsTimesDelta = (helpers::toArray(delta) * (*layer).weights().transposed()).transpose();
        assert(weightsTimesDelta.cols() == 1);
        delta = weightsTimesDelta.column(0) * layer->sigmoidPrime();
                //helpers::toVector(helpers::toArray(((*(layer+1)).weights() * delta).column(0)) * layer->sigmoidPrime());
        *deltaNablaBias = delta;
        if (deltaNablaWeight != deltaNablaWeights.rend()) {
            *deltaNablaWeight = (Matrix((delta)) * helpers::toArray(*(activation)));
        }
        ++deltaNablaBias; ++deltaNablaWeight; ++activation; ++layer;
    }
    return {deltaNablaBiases, deltaNablaWeights};
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
