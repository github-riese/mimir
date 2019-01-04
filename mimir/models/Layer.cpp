#include "Layer.h"
#include "Layer.h"

#include <algorithm>
#include <exception>
#include <numeric>
#include <strstream>
#include <valarray>
#include <cstdlib>

#include "helpers/activations.h"
#include "helpers/helpers.h"
#include "helpers/math.h"

using namespace mimir::helpers::math;

namespace mimir {
namespace models {

Layer::Layer(helpers::Activation *activate) :
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

std::vector<double> Layer::hypothesis()
{
    if (_dirty) {
        if (!_isInputLayer) {
            auto z = _inputs + _biases;
            _values.assign(z.size(), 0);
            struct activate {
                helpers::Activation *f;
                double operator()(double z) { return  f->activate(z); }
            };
            std::transform(z.begin(), z.end(), _values.begin(), activate{_activator});
        }
        _dirty = false;
    }
    return _values;
}

std::vector<double> Layer::hypothesis() const
{
    return _values;
}

bool Layer::connect(Layer &next)
{
    if (_weights.rows() > 0) {
        return false;
    }
    auto nextLayerNeuronCount = next._inputs.size();
    auto nextSize = next.size();
    _weights = Matrix{
            _inputs.size(),
            nextLayerNeuronCount,
            [nextSize](auto, auto) ->auto {
                return
                        static_cast<double>(std::rand()%200)/10000. - .001 / (static_cast<double>(nextSize*nextSize));
            }
    };
    _isConnected = true;
    _nextLayerSize = nextSize;
    _dirty = true;
    return true;
}

double Layer::weight(size_t idxMyNeuron, size_t idxNextLayerNeuron) const
{
    if (!_isConnected) {
        throw std::logic_error("not connected");
    }
    if (idxMyNeuron >= size() || idxNextLayerNeuron >= _nextLayerSize) {
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
    struct derivative {
        helpers::Activation *f;
        double operator()(double v) { return f->derivative(v); }
    };
    std::transform(result.begin(), result.end(), result.begin(), derivative{_activator});
    return result;
}

std::vector<double> Layer::run()
{
    if (!_isConnected) {
        throw std::logic_error("can't run a layer without subsequent layer.");
    }
    std::vector<double> vals;
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

helpers::Activation *Layer::activation() const
{
    return _activator;
}

void Layer::setActivation(helpers::Activation *act)
{
    _activator = act;
}



} // namespace models
} // namespace mimir
