#include "Layer.h"

#include <algorithm>
#include <exception>
#include <future>
#include <iterator>

namespace mimir {
namespace models {

Layer::Layer()
{

}

void Layer::addNeuron(const std::shared_ptr<Neuron> &neuron)
{
    _neurons.push_back(neuron);
}

std::vector<double> Layer::values() const
{
    std::vector<double> values;
    std::transform(_neurons.begin(), _neurons.end(),
                   std::back_inserter(values),
                   [](std::shared_ptr<Neuron>const &n) ->double
                    {
                        return *n;
                    });
    return values;
}

bool Layer::connect(const Layer &next)
{
    if (_edges.size() > 0) {
        return false;
    }
    auto nextLayerNeurons = next.neurons();
    std::for_each(nextLayerNeurons.begin(), nextLayerNeurons.end(),
                  [this](auto nextLayerNeuron){
        std::for_each(_neurons.begin(), _neurons.end(),
                      [this, &nextLayerNeuron](auto myNeuron){
            _edges.push_back({myNeuron, nextLayerNeuron, 1.0});
        });
    });
    _nextLayer = &next;
    return true;
}

const std::vector<std::shared_ptr<Neuron> > &Layer::neurons() const
{
    return _neurons;
}

Edge &Layer::edge(long idxMyNeuron, long idxNextLayerNeuron)
{
    if (idxMyNeuron < 0 || idxNextLayerNeuron < 0) {
        throw std::logic_error("indices must be at least zero");
    }
    if (_nextLayer == nullptr) {
        throw std::logic_error("not connected");
    }
    long numNextNodes = static_cast<long>(_nextLayer->neurons().size());
    if (idxMyNeuron >= static_cast<long>(_neurons.size()) || idxNextLayerNeuron >= static_cast<long>(numNextNodes)) {
        throw std::out_of_range("no such edge");
    }
    auto first = _edges.begin();
    std::advance(first, idxMyNeuron*numNextNodes + idxNextLayerNeuron);
    return  *first;
}

std::vector<Edge> Layer::operator[](long neuronIdx) const
{
    if (neuronIdx >= static_cast<long>(_neurons.size()) || _nextLayer == nullptr) {
        return {};
    }
    long stride = static_cast<long>(_nextLayer->_neurons.size());
    std::vector<Edge> outEdges = {};
    auto first = _edges.begin();
    std::advance(first, stride * neuronIdx);
    auto last = first;
    std::advance(last, stride);
    std::copy(first, last, std::back_inserter(outEdges));
    return  outEdges;
}

void Layer::setValues(const std::vector<double> &values)
{
    if (values.size() != _neurons.size()) {
        throw std::logic_error("wrong number of inputs for setValues");
    }
    auto value = values.begin();
    auto neuron = _neurons.begin();
    while (value != values.end()) {
        (*neuron)->addInput(*value);
        ++neuron; ++value;
    }
}

void Layer::setBiases(const std::vector<double> &biasValues)
{
    if (biasValues.size() != _neurons.size()) {
        throw std::logic_error("wrong number of biases for layer");
    }
    auto bias = biasValues.begin();
    auto neuron = _neurons.begin();
    while (bias != biasValues.end()) {
        (*neuron)->setBias(*bias);
        ++neuron; ++bias;
    }
}

void Layer::setWeights(const std::vector<double> &weights)
{
    if (weights.size() != _edges.size()) {
        throw std::logic_error("wrong number of weights for layer");
    }
    auto weight = weights.begin();
    auto edge = _edges.begin();
    while (weight != weights.end()) {
        (*edge).setWeight(*weight);
        ++weight; ++edge;
    }
}

Neuron &Layer::neuron(long index)
{
    if (index >= static_cast<long>(_neurons.size())) {
        throw std::out_of_range("no such neuron in layer");
    }
    return *_neurons.at(static_cast<size_t>(index)).get();
}

void Layer::run()
{
    std::for_each(_edges.begin(), _edges.end(), [](auto edge) -> void {
        edge.propagate();
    });
}

bool Layer::isConnected() const
{
    return _edges.size() > 0;
}

void Layer::reset()
{
    std::for_each(_neurons.begin(), _neurons.end(),
                  [](auto neuron){
        neuron->reset();
    });
}

} // namespace models
} // namespace mimir
