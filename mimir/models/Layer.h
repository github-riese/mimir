#ifndef LAYER_H
#define LAYER_H

#include <memory>
#include <valarray>
#include <vector>

#include "models/Neuron.h"
#include "models/Matrix.h"

namespace mimir {
namespace models {

class Layer
{
public:
    Layer() = default;
    void addNeuron(const Neuron &neuron);
    std::vector<double> values();
    bool connect(Layer &next);
    std::vector<Neuron> const &neurons() const;
    std::vector<Neuron> &neurons();
    double weight(size_t idxMyNeuron, size_t idxNextLayerNeuron) const;
    const Matrix &weights() const;
    void addInput(std::vector<double> const &);
    std::valarray<double> input() const;
    std::valarray<double> biases() const;
    void setBiases(std::valarray<double> const &);
    void changeBiases(std::valarray<double> const &);
    void setWeights(const Matrix &);
    void changeWeights(const Matrix &);
    std::valarray<double> zValues() const;
    std::valarray<double> sigmoidPrime() const;
    Neuron &neuron(long index);
    void run();
    bool isConnected() const;
    void reset();
    size_t size() const noexcept;
    size_t nextSize() const noexcept;
private:
    std::vector<Neuron> _neurons;
    Layer * _nextLayer = nullptr;
    std::vector<double> _values;
    Matrix _weights;
    bool _dirty = false;
};

} // namespace models
} // namespace mimir

#endif // LAYER_H
