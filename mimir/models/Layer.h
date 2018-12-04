#ifndef LAYER_H
#define LAYER_H

#include "models/Edge.h"

#include <memory>
#include <vector>

namespace mimir {
namespace models {

class Layer
{
public:
    Layer();
    void addNeuron(std::shared_ptr<Neuron> const &neuron);
    std::vector<double> values() const;
    bool connect(Layer const &next);
    std::vector<std::shared_ptr<Neuron>> const &neurons() const;
    Edge &edge(long idxMyNeuron, long idxNextLayerNeuron);
    std::vector<Edge> operator[](long neuronIdx) const;
    void setValues(std::vector<double> const &);
    void setBiases(std::vector<double> const &);
    void setWeights(std::vector<double> const &);
    Neuron &neuron(long index);
    void run();
    bool isConnected() const;
    void reset();
private:
    std::vector<Edge> _edges;
    std::vector<std::shared_ptr<Neuron>> _neurons;
    Layer const* _nextLayer = nullptr;
};

} // namespace models
} // namespace mimir

#endif // LAYER_H
