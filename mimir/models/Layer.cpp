#include "Layer.h"

#include <algorithm>
#include <exception>
#include <numeric>
#include <strstream>
#include <valarray>
#include <cstdlib>

#include "helpers/helpers.h"

namespace mimir {
namespace models {

void Layer::addNeuron(const Neuron &neuron)
{
    _neurons.push_back(neuron);
    _dirty = true;
}

std::vector<double> Layer::values()
{
    if (_dirty) {
        _dirty = false;
        std::valarray<double> values(_neurons.size());
        size_t idx = 0;
        for (auto& neuron : _neurons)
            values[idx++] = neuron;
        _values.resize(values.size());
        std::copy(std::begin(values), std::end(values), std::begin(_values));
    }
    return _values;
}

bool Layer::connect(Layer &next)
{
    if (_weights.rows() > 0) {
        return false;
    }
    auto nextLayerNeuronCount = next.neurons().size();
    _weights = Matrix{_neurons.size(), nextLayerNeuronCount, [](auto, auto) ->auto { return static_cast<double>(rand()%20000)/10000. -1.;}};
    _nextLayer = &next;
    _dirty = true;
    return true;
}

const std::vector<Neuron> &Layer::neurons() const
{
    return _neurons;
}

std::vector<Neuron> &Layer::neurons()
{
    return _neurons;
}

double Layer::weight(size_t idxMyNeuron, size_t idxNextLayerNeuron) const
{
    if (_nextLayer == nullptr) {
        throw std::logic_error("not connected");
    }
    size_t numNextNodes = _nextLayer->neurons().size();
    if (idxMyNeuron >= _neurons.size() || idxNextLayerNeuron >= numNextNodes) {
        throw std::out_of_range("no such edge");
    }
    return _weights.value(idxNextLayerNeuron, idxMyNeuron);
}

const Matrix &Layer::weights() const
{
    return _weights;
}

void Layer::addInput(const std::vector<double> &values)
{
    if (values.size() != _neurons.size()) {
        throw std::logic_error("wrong number of inputs for setValues");
    }
    _dirty = true;
    auto value = values.begin();
    auto neuron = _neurons.begin();
    while (value != values.end()) {
        (*neuron) << (*value);
        ++neuron; ++value;
    }
}

std::valarray<double> Layer::input() const
{
    std::valarray<double> result(_neurons.size());
    size_t idx = 0;
    for (auto neuron : _neurons) {
        result[idx++] = neuron.input();
    }
    return result;
}

std::valarray<double> Layer::biases() const
{
    std::valarray<double> result(_neurons.size());
    unsigned i = 0;
    for (auto neuron : neurons()) {
        result[i++] = neuron.bias();
    }
    return result;
}

void Layer::setBiases(const std::valarray<double> &biasValues)
{
    if (biasValues.size() != _neurons.size()) {
        throw std::logic_error("wrong number of biases for layer");
    }
    _dirty = true;
    auto bias = std::begin(biasValues);
    auto neuron = std::begin(_neurons);
    auto end = std::end(biasValues);
    while (bias != end) {
        (*neuron).setBias(*bias);
        ++neuron; ++bias;
    }
}

void Layer::changeBiases(const std::valarray<double> &deltas)
{
    if (deltas.size() != _neurons.size()) {
        throw std::logic_error("wrong number of biases for layer");
    }
    _dirty = true;
    auto delta = std::begin(deltas);
    auto end = std::end(deltas);
    auto neuron = std::begin(_neurons);
    while (delta != end) {
        (*neuron).setBias((*neuron).bias() - *delta);
        ++neuron; ++delta;
    }
}

void Layer::setWeights(const Matrix &weights)
{
    if (_nextLayer == nullptr) {
        throw std::logic_error("no weights applyable on unconnected layer.");
    }
    if (weights.rows() != _neurons.size()) {
        throw std::logic_error("wrong number of weights for layer.");
    }
    if (weights.cols() != _nextLayer->_neurons.size()) {
        throw std::logic_error("wrong number of weights for subsequent layer.");
    }
    _weights = weights;
    _dirty = true;
}

void Layer::changeWeights(const Matrix &delta)
{
    _weights -= delta.column(0);
}

std::valarray<double> Layer::zValues() const
{
    std::valarray<double> result(_neurons.size());
    unsigned idx = 0;
    for (auto neuron : _neurons) {
        result[idx++] = neuron.z();
    }
    return result;
}

std::valarray<double> Layer::sigmoidPrime() const
{
    std::valarray<double> result(size());
    size_t idx = 0;
    for (auto neuron : _neurons) {
        result[idx++] = neuron.sigmoidPrime();
    }
    return result;
}

Neuron &Layer::neuron(long index)
{
    if (index >= static_cast<long>(_neurons.size())) {
        throw std::out_of_range("no such neuron in layer");
    }
    return _neurons.at(static_cast<size_t>(index));
}

void Layer::run()
{
    if (_nextLayer == nullptr) {
        throw std::logic_error("can't run a layer without subsequent layer.");
    }
    Matrix nextInput = _weights.transposed() * values();
    _nextLayer->reset();
    _nextLayer->addInput(nextInput.column(0));
}

bool Layer::isConnected() const
{
    return _nextLayer != nullptr;
}

void Layer::reset()
{
    _dirty = true;
    std::for_each(_neurons.begin(), _neurons.end(),
        [](auto &neuron){
            neuron.resetInput();
    });
    if (_nextLayer != nullptr) {
        _nextLayer->reset();
    }
}

size_t Layer::size() const noexcept
{
    return _neurons.size();
}

size_t Layer::nextSize() const noexcept
{
    return _nextLayer == nullptr ? 0 : _nextLayer->size();
}

} // namespace models
} // namespace mimir
