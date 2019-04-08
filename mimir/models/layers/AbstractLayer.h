#ifndef LAYER_H
#define LAYER_H

#include <memory>
#include <valarray>
#include <vector>
#include <string>

#include <mimir/models/Matrix.h>
#include <mimir/models/activation/ActivationInterface.h>

namespace mimir {
namespace models {

class AbstractLayer
{
public:
    explicit AbstractLayer() = default;
    virtual ~AbstractLayer() = default;
    /**
     * @brief creates a new layer with the given activation
     */
    /**
     * @brief adds a new node to this layer.
     * @param bias value for the new node (which will be, of course, ignored if _isInputLayer = true)
     */
    void addNode(double bias = 0., std::valarray<double> const &weights = {});
    std::vector<double> hypothesis();
    std::vector<double> hypothesis() const;
    bool connect(const AbstractLayer &next);
    bool reconnect(AbstractLayer const &next, const std::vector<double> &weights = {});
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
    std::vector<double> run();
    bool isConnected() const;
    bool isInputLayer() const;
    size_t size() const noexcept;
    size_t nextSize() const noexcept;
    void setIsInput(bool isInput);
    bool isOutputLayer() const;
    void setIsOutputLayer(bool isOutputLayer);

    virtual std::string name() const noexcept = 0;
    virtual std::vector<double> gradientOnLoss(std::vector<double> const &expectation, AbstractLayer const *previousLayer) noexcept = 0;
    virtual std::vector<double> gradient(AbstractLayer const *previous) noexcept = 0;
protected:
    virtual void activate() noexcept = 0;

protected:
    std::vector<double> _inputs;
    std::vector<double> _biases;
    std::vector<double> _hypothesis;
    Matrix _weights;
private:
    size_t _nextLayerSize = 0u;
    bool _isConnected = false;
    bool _dirty = false;
    bool _isInputLayer = false;
    bool _isOutputLayer = false;
};

} // namespace models
} // namespace mimir

#endif // LAYER_H
