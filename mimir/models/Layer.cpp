#include "Layer.h"
#include "Layer.h"

#include <algorithm>
#include <exception>
#include <numeric>
#include <strstream>
#include <valarray>
#include <cstdlib>

#include "helpers/helpers.h"
#include "helpers/math.h"

using namespace mimir::helpers::math;

namespace mimir {
namespace models {

Layer::Layer(std::shared_ptr<helpers::Activation> activate) :
    _activator(activate)
{
}

void Layer::addNeuron(double bias)
{
    _dirty = true;
    _biases.push_back(bias);
    _inputs.push_back(0);
    _values.push_back(0);
}

std::vector<double> Layer::values()
{
    if (_dirty) {
        if (!_isInputLayer) {
            auto z = _inputs + _biases;
            _values.assign(z.size(), 0);
            std::transform(z.begin(), z.end(), _values.begin(), [this](double z) -> double {return _activator->activate(z);});
        }
        _dirty = false;
    }
    return _values;
}

bool Layer::connect(Layer &next)
{
    if (_weights.rows() > 0) {
        return false;
    }
    auto nextLayerNeuronCount = next._inputs.size();
    _weights = Matrix{_inputs.size(), nextLayerNeuronCount, [](auto, auto) ->auto { return static_cast<double>(std::rand()%100)/10000.;}};
    _nextLayer = &next;
    _dirty = true;
    return true;
}

double Layer::weight(size_t idxMyNeuron, size_t idxNextLayerNeuron) const
{
    if (_nextLayer == nullptr) {
        throw std::logic_error("not connected");
    }
    size_t numNextNodes = _nextLayer->size();
    if (idxMyNeuron >= size() || idxNextLayerNeuron >= numNextNodes) {
        throw std::out_of_range("no such edge");
    }
    return _weights.value(idxNextLayerNeuron, idxMyNeuron);
}

const Matrix &Layer::weights() const
{
    return _weights;
}

void Layer::setInput(const std::vector<double> &values)
{
    if (values.size() != size()) {
        throw std::logic_error("wrong number of inputs for setValues");
    }
    if (_isInputLayer) {
        _values = values;
        _dirty = false;
    } else {
        _dirty = true;
        _inputs = values;
    }
}

std::vector<double> Layer::input() const
{
    return _inputs;
}

std::vector<double> Layer::biases() const
{
    return _biases;
}

void Layer::setBiases(const std::vector<double> &biasValues)
{
    if (biasValues.size() != _biases.size()) {
        throw std::logic_error("wrong number of biases for layer");
    }
    _dirty = true;
    _biases = biasValues;
}

void Layer::setBias(size_t neuron, double value)
{
    _biases[neuron] = value;
}

void Layer::changeBiases(const std::vector<double> &deltas)
{
    if (deltas.size() != _biases.size()) {
        throw std::logic_error("wrong number of biases for layer");
    }
    _dirty = true;
    _biases -= deltas;
}

void Layer::setWeights(const Matrix &weights)
{
    if (_nextLayer == nullptr) {
        throw std::logic_error("no weights applyable on unconnected layer.");
    }
    if (weights.rows() != _inputs.size()) {
        throw std::logic_error("wrong number of weights for layer.");
    }
    if (weights.cols() != _nextLayer->_inputs.size()) {
        throw std::logic_error("wrong number of weights for subsequent layer.");
    }
    _weights = weights;
    _dirty = true;
}

void Layer::setWeight(size_t neuron, size_t nextLayerNeuron, double value)
{
    _weights.setValue(neuron, nextLayerNeuron, value);
}

void Layer::changeWeights(const Matrix &delta)
{
    _weights -= delta.column(0);
}

std::vector<double> Layer::zValues() const
{
    return _inputs + _biases;
}

std::vector<double> Layer::sigmoidPrime() const
{
    std::vector<double> result = zValues();
    std::transform(result.begin(), result.end(), result.begin(), [this](double z) -> double { return _activator->derivative(z); });
    return result;
}

void Layer::run()
{
    if (_nextLayer == nullptr) {
        throw std::logic_error("can't run a layer without subsequent layer.");
    }
    std::vector<double> vals;
    // line vector from input
    vals = (helpers::toArray(values()) * _weights).transpose().column(0);
    _nextLayer->setInput(vals);
}

bool Layer::isConnected() const
{
    return _nextLayer != nullptr;
}

bool Layer::isInputLayer() const
{
    return _isInputLayer;
}

size_t Layer::size() const noexcept
{
    return _inputs.size();
}

size_t Layer::nextSize() const noexcept
{
    return _nextLayer == nullptr ? 0 : _nextLayer->size();
}

void Layer::setIsInput(bool isInput)
{
    _isInputLayer = isInput;
}

} // namespace models
} // namespace mimir
