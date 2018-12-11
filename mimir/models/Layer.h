#ifndef LAYER_H
#define LAYER_H

#include <memory>
#include <valarray>
#include <vector>

#include "models/Neuron.h"

namespace mimir {
namespace models {

class Layer
{
public:
    Layer() = default;
    void addNeuron(const Neuron &neuron);
    std::valarray<double> values();
    bool connect(Layer &next);
    std::vector<Neuron> const &neurons() const;
    std::vector<Neuron> &neurons();
    double weight(size_t idxMyNeuron, size_t idxNextLayerNeuron) const;
    std::vector<std::valarray<double>> const &weights() const;
    void addInput(std::vector<double> const &);
    std::valarray<double> input() const;
    void setBiases(std::valarray<double> const &);
    void setWeights(const std::vector<std::vector<double> > &);
    std::valarray<double> deriviateActivations() const;
    Neuron &neuron(long index);
    void run();
    bool isConnected() const;
    void reset();
    size_t size() const noexcept;
private:
    std::vector<Neuron> _neurons;
    Layer * _nextLayer = nullptr;
    std::valarray<double> _values;
    std::vector<std::valarray<double>> _weights;
    bool _dirty = false;
};

} // namespace models
} // namespace mimir

#endif // LAYER_H
