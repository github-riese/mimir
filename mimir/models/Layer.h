#ifndef LAYER_H
#define LAYER_H

#include <memory>
#include <valarray>
#include <vector>

#include "Matrix.h"
#include "../helpers/activations.h"

namespace mimir {
namespace models {

class Layer
{
public:
    Layer(helpers::Activation*);
    void addNeuron(double);
    std::vector<double> hypothesis();
    std::vector<double> hypothesis() const;
    bool connect(Layer &next);
    double weight(size_t idxMyNeuron, size_t idxNextLayerNeuron) const;
    const Matrix &weights() const;
    void setInput(std::vector<double> const &);
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
    std::vector<double> run();
    bool isConnected() const;
    bool isInputLayer() const;
    size_t size() const noexcept;
    size_t nextSize() const noexcept;
    void setIsInput(bool isInput);
    helpers::Activation *activation() const;
    void setActivation(helpers::Activation *act);
protected:
private:
    bool _isConnected = false;
    size_t _nextLayerSize = 0u;
    std::vector<double> _inputs;
    std::vector<double> _biases;
    std::vector<double> _values;
    Matrix _weights;
    helpers::Activation *_activator = nullptr;
    bool _dirty = false;
    bool _isInputLayer = false;
};

} // namespace models
} // namespace mimir

#endif // LAYER_H
