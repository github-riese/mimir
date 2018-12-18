#include "NeuronNet.h"

#include <algorithm>
#include <iterator>
#include <cstdlib>

#include "helpers/math.h"
#include "helpers/helpers.h"

using mimir::models::Layer;
using mimir::models::Matrix;

namespace mimir {
namespace services {

template<typename T>
std::vector<T> operator*(T const &left, std::vector<T> const &right)
{
    std::vector<T> result(right);
    std::transform(result.begin(), result.end(), result.begin(), [left](T const &v) -> T { return  left * v;});
    return result;
}

template<typename T>
std::vector<T> operator*(std::vector<T> const &left, std::vector<T> const &right)
{
    std::vector<T> result(right);
    std::transform(left.begin(), left.end(), right.begin(), result.begin(), [](T const &l, T const &r) -> T { return  l * r;});
    return result;
}

template<typename T>
std::vector<T> operator*(T const &left, std::valarray<T> const &right)
{
    std::vector<T> result(right.size());
    std::transform(std::begin(right), std::end(right), result.begin(), [left](T const &v) -> T { return  left * v;});
    return result;
}

NeuronNet::NeuronNet(long inputs, long outputs) :
    _output()
{
    Layer input;
    for (auto n = 0; n < inputs; ++n) {
        models::Neuron neuron;
        neuron.setBias(static_cast<double>(rand()%2000)/1000. -1.);
        //neuron.setBias(0);
        input.addNeuron(neuron);
    }
    _layers.push_back(input);
    Layer output;
    for (auto n = 0; n < outputs; ++n) {
        models::Neuron neuron;
        neuron.setBias(static_cast<double>(rand()%2000)/1000. -1.);
        //neuron.setBias(0);
        output.addNeuron(neuron);
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
        models::Neuron neuron;
        neuron.setBias(static_cast<double>(rand()%2000)/1000. -1.);
        l.addNeuron(neuron);
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
    auto output = layer + static_cast<long>(_layers.size() - 1);
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
void NeuronNet::backPropagate(const std::vector<std::vector<double>> &results, std::vector<std::vector<double>> const &expectations, double eta)
{
    std::vector<std::vector<double>> nablaBiases;
    std::vector<Matrix> nablaWeights;

    std::vector<std::vector<double>> deltaNablaBiases;
    std::vector<Matrix> deltaNablaWeights;

    std::vector<std::vector<double>> biases;
    std::vector<Matrix> weights;

    for (auto layer : _layers) {
        nablaBiases.push_back(std::vector<double>(layer.size()));
        if (layer.isConnected()) {
            nablaWeights.push_back(Matrix(layer.size(), layer.nextSize() == 0 ? 1 : layer.nextSize(), .0));
        }
    }

    auto x = results.begin();
    auto y = expectations.begin();
    while (x != results.end()) {
        auto deltas = deltaNabla(helpers::toVector(helpers::toArray(*x) - helpers::toArray(*y)));
        deltaNablaBiases = std::get<0>(deltas);
        deltaNablaWeights = std::get<1>(deltas);
        nablaBiases = addVectors(nablaBiases, deltaNablaBiases);
        nablaWeights = addMatices(nablaWeights, deltaNablaWeights);

        ++x; ++y;
    }

    //eta = eta / static_cast<double>(results.size());

    auto nablaWeight = nablaWeights.begin();
    auto nablaBias = nablaBiases.begin();
    for (auto &layer : _layers) {
        layer.setBiases(layer.biases() - helpers::toArray(*nablaBias) * eta);
        if (layer.isConnected()) {
            Matrix t(layer.weights());
            t -= (*nablaWeight) * eta;
            layer.setWeights(t);
        }
        ++nablaWeight; ++nablaBias;
    }

}

size_t NeuronNet::outputSize() const
{
    return _layers.back().size();
}

std::tuple<std::vector<std::vector<double> >, std::vector<models::Matrix> >
  NeuronNet::deltaNabla(std::vector<double> const &costDerivative) const
{
    std::vector<std::vector<double>> deltaNablaBiases;
    std::vector<Matrix> deltaNablaWeights;
    std::vector<std::vector<double>> activations;

    for (auto layer : _layers) {
        deltaNablaBiases.push_back(std::vector<double>(layer.size()));
        if (layer.isConnected()) {
            deltaNablaWeights.push_back(Matrix(layer.size(), layer.nextSize() == 0 ? 1 : layer.nextSize(), 0.));
        }
        activations.push_back(layer.values());
    }

    auto deltaNablaBias = deltaNablaBiases.rbegin();
    auto deltaNablaWeight = deltaNablaWeights.rbegin();
    auto activation = activations.rbegin();

    auto layer = _layers.rbegin();

    auto sigPrim = layer->sigmoidPrime();
    auto delta = helpers::toVector(helpers::toArray(costDerivative) * layer->sigmoidPrime());
    *deltaNablaBias = delta;
    *deltaNablaWeight = (Matrix((delta)) * helpers::toArray(*(activation )));
    ++deltaNablaBias; ++deltaNablaWeight; ++activation;

    ++layer;
    while (deltaNablaBias != deltaNablaBiases.rend()) {
        auto wTimesD = (*layer).weights() * delta;
        delta = wTimesD.column(0) * helpers::toVector(layer->sigmoidPrime());
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
