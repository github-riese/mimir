#include "Layer.h"

#include <algorithm>
#include <exception>
#include <numeric>
#include <strstream>
#include <valarray>

namespace mimir {
namespace models {

void Layer::addNeuron(const Neuron &neuron)
{
    _neurons.push_back(neuron);
    _dirty = true;
}

std::valarray<double> Layer::values()
{
    if (_dirty) {
        _dirty = false;
        std::valarray<double> values(_neurons.size());
        size_t idx = 0;
        for (auto neuron : _neurons)
            values[idx++] = neuron;
        _values = values;
    }
    return _values;
}

bool Layer::connect(Layer &next)
{
    if (_weights.size() > 0) {
        return false;
    }
    _weights.assign(_neurons.size(), {});
    auto nextLayerNeuronCount = next.neurons().size();
    std::for_each(_weights.begin(), _weights.end(),
                  [nextLayerNeuronCount](auto & w){
                        w = std::valarray<double>(1., nextLayerNeuronCount);
                    });
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
    return _weights.at(idxMyNeuron)[idxNextLayerNeuron];
}

const std::vector<std::valarray<double> > &Layer::weights() const
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

void Layer::setWeights(const std::vector<std::vector<double>> &weights)
{
    if (_nextLayer == nullptr) {
        throw std::logic_error("no weights applyable on unconnected layer.");
    }
    if (weights.size() != _neurons.size()) {
        throw std::logic_error("wrong number of weights for layer.");
    }
    auto newWeights = weights.begin();
    auto oldWeights = _weights.begin();
    while (newWeights != weights.end()) {
        if ((*newWeights).size() != (*oldWeights).size()) {
            throw std::logic_error((std::strstream() << "in " << std::distance(newWeights, weights.begin()) << "th node: wrong number of weights.").str());
        }
    }
    _dirty = true;
    _weights.clear();
    for(auto weight : weights) {
        _weights.push_back({weight.data(), weight.size()});
    }
}

std::valarray<double> Layer::deriviateActivations() const
{
    std::valarray<double> result(size());
    size_t idx = 0;
    for (auto neuron : _neurons) {
        result[idx++] = neuron.deriviateActivation();
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
    std::valarray<double> intermediate(0., _nextLayer->size());
    for (auto weight : _weights) {
        for (auto value : values()) {
            intermediate += weight * value;
        }
    }
    std::vector<double> nextInput(std::begin(intermediate), std::end(intermediate));
    _nextLayer->addInput(nextInput);
}

bool Layer::isConnected() const
{
    return _weights.size() > 0;
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

} // namespace models
} // namespace mimir
