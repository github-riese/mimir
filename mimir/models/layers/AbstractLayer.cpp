#include "AbstractLayer.h"

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

void AbstractLayer::addNode(double bias, const std::valarray<double> &weights)
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
            _weights.addRow(std::valarray<double>(0., _nextLayerSize));
        } else if (weights.size() == 1) {
            _weights.addRow(std::valarray<double>(weights[0], _nextLayerSize));
        } else {
            _weights.addRow(weights);
        }
    }
}

std::vector<double> AbstractLayer::hypothesis()
{
    if (_dirty) {
        if (!_isInputLayer) {
            _hypothesis = _inputs + _biases;
            activate();
        }
        _dirty = false;
    }
    return _hypothesis;
}

std::vector<double> AbstractLayer::hypothesis() const
{
    return _hypothesis;
}

bool AbstractLayer::connect(AbstractLayer const &next)
{
    if (_isConnected) {
        return false;
    }
    std::random_device randomDevice;
    std::mt19937 randomnessGenerator(randomDevice());
    std::uniform_real_distribution<> normalDistribution(.0, .1);
    auto nextLayerNeuronCount = next._inputs.size();
    auto nextSize = next.size();
    if (nextSize > 0) {
        double nextSizeSquared = static_cast<double>(nextSize*nextSize);
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

bool AbstractLayer::reconnect(const AbstractLayer &next, std::vector<double> const &weights)
{
    if (!isConnected()) {
        return connect(next);
    }

    long extraColumns = static_cast<long>(next.size() - _nextLayerSize);
    if (extraColumns < 0) {
        return false;
    }
    double singleValue = weights.size() == 1 ? weights.front() : 0;
    while (extraColumns-- > 0) {
        _weights.addColumn(-1u, (weights.size() == _inputs.size()) ? weights : std::vector<double>(_inputs.size(), singleValue));
    }
    _nextLayerSize = next.size();
    _dirty = true;
    return true;
}

double AbstractLayer::weight(size_t idxMyNeuron, size_t idxNextLayerNeuron) const
{
    if (!_isConnected) {
        throw std::logic_error("Not connected");
    }
    if (idxMyNeuron >= size() || idxNextLayerNeuron >= _nextLayerSize) {
        throw std::out_of_range("No such edge.");
    }
    return _weights.value(idxNextLayerNeuron, idxMyNeuron);
}

const Matrix &AbstractLayer::weights() const
{
    return _weights;
}

void AbstractLayer::setInput(const std::vector<double> &values)
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

std::vector<double> AbstractLayer::input() const
{
    return _inputs;
}

std::vector<double> AbstractLayer::biases() const
{
    return _biases;
}

void AbstractLayer::setBiases(const std::vector<double> &biasValues)
{
    if (biasValues.size() != _biases.size()) {
        throw std::logic_error("wrong number of biases for layer");
    }
    _dirty = true;
    _biases = biasValues;
}

void AbstractLayer::setBias(size_t neuron, double value)
{
    _biases[neuron] = value;
}

void AbstractLayer::changeBiases(const std::vector<double> &deltas)
{
    if (deltas.size() != _biases.size()) {
        throw std::logic_error("wrong number of biases for layer");
    }
    _dirty = true;
    _biases -= deltas;
}

void AbstractLayer::setWeights(const Matrix &weights)
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

void AbstractLayer::setWeight(size_t neuron, size_t nextLayerNeuron, double value)
{
    _weights.setValue(neuron, nextLayerNeuron, value);
}

void AbstractLayer::changeWeights(const Matrix &delta)
{
    _weights -= delta.column(0);
}

std::vector<double> AbstractLayer::zValues() const
{
    return _inputs + _biases;
}

std::vector<double> AbstractLayer::run()
{
    if (!_isConnected) {
        throw std::logic_error("can't run a layer without subsequent layer.");
    }
    std::vector<double> vals;
    vals = (_weights.transposed() * hypothesis()).column(0);
    return vals;
}

bool AbstractLayer::isConnected() const
{
    return _isConnected;
}

bool AbstractLayer::isInputLayer() const
{
    return _isInputLayer;
}

size_t AbstractLayer::size() const noexcept
{
    return _inputs.size();
}

size_t AbstractLayer::nextSize() const noexcept
{
    return _nextLayerSize;
}


bool AbstractLayer::isOutputLayer() const
{
    return _isOutputLayer;
}

void AbstractLayer::setIsOutputLayer(bool isOutputLayer)
{
    _isOutputLayer = isOutputLayer;
}

} // namespace models
} // namespace mimir