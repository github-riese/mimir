#ifndef LAYER_H
#define LAYER_H

#include <memory>
#include <valarray>
#include <vector>

#include "Matrix.h"
#include <mimir/models/activation/ActivationInterface.h>

namespace mimir {
namespace models {

class Layer
{
public:
    explicit Layer() = default;
    /**
     * @brief creates a new layer with the given activation
     */
    Layer(models::activation::ActivationInterface*);
    /**
     * @brief adds a new node to this layer.
     * @param bias value for the new node (which will be, of course, ignored if _isInputLayer = true)
     */
    void addNode(float bias = 0., std::valarray<float> const &weights = {});
    std::vector<float> hypothesis();
    std::vector<float> hypothesis() const;
    bool connect(const Layer &next);
    bool reconnect(Layer const &next, const std::vector<float> &weights = {});
    float weight(size_t idxMyNeuron, size_t idxNextLayerNeuron) const;
    const Matrix &weights() const;
    void setInput(std::vector<float> const &);
    std::vector<float> input() const;
    std::vector<float> biases() const;
    void setBiases(const std::vector<float> &);
    void setBias(size_t neuron, float value);
    void changeBiases(const std::vector<float> &);
    void setWeights(const Matrix &);
    void setWeight(size_t neuron, size_t nextLayerNeuron, float value);
    void changeWeights(const Matrix &);
    std::vector<float> zValues() const;
    std::vector<float> run();
    bool isConnected() const;
    bool isInputLayer() const;
    size_t size() const noexcept;
    size_t nextSize() const noexcept;
    void setIsInput(bool isInput);
    models::activation::ActivationInterface *activation() const;
    void setActivation(models::activation::ActivationInterface *act);
    bool isOutputLayer() const;
    void setIsOutputLayer(bool isOutputLayer);

protected:
private:
    bool _isConnected = false;
    size_t _nextLayerSize = 0u;
    std::vector<float> _inputs;
    std::vector<float> _biases;
    std::vector<float> _hypothesis;
    Matrix _weights;
    models::activation::ActivationInterface *_activator = nullptr;
    bool _dirty = false;
    bool _isInputLayer = false;
    bool _isOutputLayer = false;
};

} // namespace models
} // namespace mimir

#endif // LAYER_H
