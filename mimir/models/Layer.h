#ifndef LAYER_H
#define LAYER_H

#include <memory>
#include <valarray>
#include <vector>

#include "models/Matrix.h"

namespace mimir {
namespace models {

class Layer
{
public:
    Layer() = default;
    void addNeuron(double);
    std::vector<double> values();
    bool connect(Layer &next);
    double weight(size_t idxMyNeuron, size_t idxNextLayerNeuron) const;
    const Matrix &weights() const;
    void addInput(std::vector<double> const &);
    std::vector<double> input() const;
    std::vector<double> biases() const;
    void setBiases(const std::vector<double> &);
    void setBias(size_t neuron, double value);
    void changeBiases(const std::vector<double> &);
    void setWeights(const Matrix &);
    void setWeight(size_t neuron, size_t nextLayerNeuron, double value);
    void changeWeights(const Matrix &);
    std::vector<double> zValues() const;
    std::vector<double> sigmoidPrime() const;
    void run();
    bool isConnected() const;
    void reset();
    size_t size() const noexcept;
    size_t nextSize() const noexcept;
    void setIsInput(bool isInput);
protected:
    double activate(double) const;
    double derivativeActivate(double) const;
private:
    Layer * _nextLayer = nullptr;
    std::vector<double> _inputs;
    std::vector<double> _biases;
    std::vector<double> _values;
    Matrix _weights;
    bool _dirty = false;
    bool _isInputLayer = false;
};

} // namespace models
} // namespace mimir

#endif // LAYER_H
