#include "Layer.h"

#include <algorithm>
#include <exception>
#include <future>
#include <iterator>
#include <strstream>

namespace mimir {
namespace models {

Layer::Layer()
{

}

void Layer::addNeuron(const Neuron &neuron)
{
    _neurons.push_back(neuron);
    _dirty = true;
}

std::vector<double> Layer::values()
{
    if (_dirty) {
        _dirty = false;
        std::vector<double> values(_neurons.begin(), _neurons.end());
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
                        w.assign(nextLayerNeuronCount, 1.);
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

double Layer::weight(size_t idxMyNeuron, size_t idxNextLayerNeuron)
{
    if (_nextLayer == nullptr) {
        throw std::logic_error("not connected");
    }
    size_t numNextNodes = _nextLayer->neurons().size();
    if (idxMyNeuron >= _neurons.size() || idxNextLayerNeuron >= numNextNodes) {
        throw std::out_of_range("no such edge");
    }
    return _weights.at(idxMyNeuron).at(idxNextLayerNeuron);
}

void Layer::setValues(const std::vector<double> &values)
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

void Layer::setBiases(const std::vector<double> &biasValues)
{
    if (biasValues.size() != _neurons.size()) {
        throw std::logic_error("wrong number of biases for layer");
    }
    _dirty = true;
    auto bias = biasValues.begin();
    auto neuron = _neurons.begin();
    while (bias != biasValues.end()) {
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
    _weights = weights;
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
    auto currentValues = values();
    auto targetWeights = _weights.begin();
    std::vector<double>nextInputs(_nextLayer->neurons().size());

    while (targetWeights != _weights.end()) {
        auto output = nextInputs.begin();
        while (output != nextInputs.end()) {
            auto input = currentValues.begin();
            auto weight = (*targetWeights).begin();

            while (input != currentValues.end()) {
                *output += *input * *weight;
                ++input; ++weight;
            }
            ++output;
        }
        ++targetWeights;
    }
    _nextLayer->setValues(nextInputs);
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

} // namespace models
} // namespace mimir
