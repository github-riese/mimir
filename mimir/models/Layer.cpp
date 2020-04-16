#include "Layer.h"

#include <algorithm>
#include <exception>
#include <numeric>
#include <sstream>
#include <valarray>
#include <random>

#include "helpers/helpers.h"
#include "helpers/math.h"

using namespace mimir::helpers::math;

namespace mimir {
namespace models {

Layer::Layer(activation::ActivationInterface *activate) :
    _activator(activate)
{
}

void Layer::addNode(float bias, const std::valarray<float> &weights)
{
    if (weights.size() > 0 && (weights.size() != _nextLayerSize || weights.size() != 1)) {
        throw std::logic_error("Wrong number of weights given on adding node.");
    }
    _dirty = true;
    _biases.push_back(bias);
    _inputs.push_back(0);
    _hypothesis.push_back(0);
    if (_nextLayerSize) {
        if (weights.size() == 0) {
            _weights.addRow(std::valarray<float>(0., _nextLayerSize));
        } else if (weights.size() == 1) {
            _weights.addRow(std::valarray<float>(weights[0], _nextLayerSize));
        } else {
            _weights.addRow(weights);
        }
    }
}

std::vector<float> Layer::hypothesis()
{
    if (_dirty) {
        if (!_isInputLayer) {
            _hypothesis = _inputs + _biases;
            _activator->activate(_hypothesis);
        }
        _dirty = false;
    }
    return _hypothesis;
}

std::vector<float> Layer::hypothesis() const
{
    return _hypothesis;
}

bool Layer::connect(Layer const &next)
{
    if (_isConnected) {
        return false;
    }
    std::random_device randomDevice;
    std::mt19937 randomnessGenerator(randomDevice());
    std::uniform_real_distribution<> normalDistribution(.0, .01);
    auto nextLayerNeuronCount = next._inputs.size();
    auto nextSize = next.size();
    if (nextSize > 0) {
        float nextSizeSquared = static_cast<float>(nextSize*nextSize);
        _weights = Matrix{
                _inputs.size(),
                nextLayerNeuronCount,
                [nextSizeSquared, &normalDistribution, &randomnessGenerator](auto, auto) ->auto {
                    return
                            normalDistribution(randomnessGenerator) / nextSizeSquared;
                }
        };
    }
    _isConnected = true;
    _nextLayerSize = nextSize;
    _dirty = true;
    return true;
}

bool Layer::reconnect(const Layer &next, std::vector<float> const &weights)
{
    if (!isConnected()) {
        return connect(next);
    }

    long extraColumns = static_cast<long>(next.size() - _nextLayerSize);
    if (extraColumns < 0) {
        return false;
    }
    float singleValue = weights.size() == 1 ? weights.front() : 0;
    while (extraColumns-- > 0) {
        _weights.addColumn(-1u, (weights.size() == _inputs.size()) ? weights : std::vector<float>(_inputs.size(), singleValue));
    }
    _nextLayerSize = next.size();
    _dirty = true;
    return true;
}

float Layer::weight(size_t idxMyNeuron, size_t idxNextLayerNeuron) const
{
    if (!_isConnected) {
        throw std::logic_error("Not connected");
    }
    if (idxMyNeuron >= size() || idxNextLayerNeuron >= _nextLayerSize) {
        throw std::out_of_range("No such edge.");
    }
    return _weights.value(idxNextLayerNeuron, idxMyNeuron);
}

const Matrix &Layer::weights() const
{
    return _weights;
}

void Layer::setInput(const std::vector<float> &values)
{
    if (values.size() != size()) {
        std::stringstream s;
        s << "Wrong number of inputs for setValues. Expected " << size() << " but got " << values.size() << ".";
        throw std::logic_error(s.str());
    }
    if (_isInputLayer) {
        _hypothesis = values;
        _dirty = false;
    } else {
        _dirty = true;
        _inputs = values;
    }
}

std::vector<float> Layer::input() const
{
    return _inputs;
}

std::vector<float> Layer::biases() const
{
    return _biases;
}

void Layer::setBiases(const std::vector<float> &biasValues)
{
    if (biasValues.size() != _biases.size()) {
        throw std::logic_error("wrong number of biases for layer");
    }
    _dirty = true;
    _biases = biasValues;
}

void Layer::setBias(size_t neuron, float value)
{
    _biases[neuron] = value;
}

void Layer::changeBiases(const std::vector<float> &deltas)
{
    if (deltas.size() != _biases.size()) {
        throw std::logic_error("wrong number of biases for layer");
    }
    _dirty = true;
    _biases -= deltas;
}

void Layer::setWeights(const Matrix &weights)
{
    if (!_isConnected) {
        throw std::logic_error("no weights applyable on unconnected layer.");
    }
    if (weights.rows() != _inputs.size()) {
        throw std::logic_error("wrong number of weights for layer.");
    }
    if (weights.cols() != _nextLayerSize) {
        throw std::logic_error("wrong number of weights for subsequent layer.");
    }
    _weights = weights;
    _dirty = true;
}

void Layer::setWeight(size_t neuron, size_t nextLayerNeuron, float value)
{
    _weights.setValue(neuron, nextLayerNeuron, value);
}

void Layer::changeWeights(const Matrix &delta)
{
    _weights -= delta.column(0);
}

std::vector<float> Layer::zValues() const
{
    return _inputs + _biases;
}

std::vector<float> Layer::run()
{
    if (!_isConnected) {
        throw std::logic_error("can't run a layer without subsequent layer.");
    }
    std::vector<float> vals;
    vals = (_weights.transposed() * hypothesis()).column(0);
    return vals;
}

bool Layer::isConnected() const
{
    return _isConnected;
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
    return _nextLayerSize;
}

void Layer::setIsInput(bool isInput)
{
    _isInputLayer = isInput;
}

activation::ActivationInterface *Layer::activation() const
{
    return _activator;
}

void Layer::setActivation(models::activation::ActivationInterface *act)
{
    _activator = act;
}

bool Layer::isOutputLayer() const
{
    return _isOutputLayer;
}

void Layer::setIsOutputLayer(bool isOutputLayer)
{
    _isOutputLayer = isOutputLayer;
}

} // namespace models
} // namespace mimir
