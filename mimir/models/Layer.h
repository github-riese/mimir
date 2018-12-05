#ifndef LAYER_H
#define LAYER_H

#include <memory>
#include <vector>

#include "models/Neuron.h"

namespace mimir {
namespace models {

class Layer
{
public:
    Layer();
    void addNeuron(const Neuron &neuron);
    std::vector<double> values();
    bool connect(Layer &next);
    std::vector<Neuron> const &neurons() const;
    std::vector<Neuron> &neurons();
    double weight(size_t idxMyNeuron, size_t idxNextLayerNeuron);
    void setValues(std::vector<double> const &);
    void setBiases(std::vector<double> const &);
    void setWeights(const std::vector<std::vector<double> > &);
    Neuron &neuron(long index);
    void run();
    bool isConnected() const;
    void reset();
private:
    std::vector<Neuron> _neurons;
    Layer * _nextLayer = nullptr;
    std::vector<double> _values;
    std::vector<std::vector<double>> _weights;
    bool _dirty = false;
};

} // namespace models
} // namespace mimir

#endif // LAYER_H
