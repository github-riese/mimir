#ifndef NET_H
#define NET_H

#include "../models/Layer.h"
#include "../models/Matrix.h"

#include <valarray>
#include <vector>
#include <string>

namespace mimir {
namespace services {

class NeuronNet
{
public:
    NeuronNet();
    NeuronNet(long inputs, long outputs, helpers::Activation *);
    void addHiddenLayer(size_t numNeurons, helpers::Activation * = nullptr);
    void appendLayer(size_t numNeurons, helpers::Activation * = nullptr);
    void connect();
    std::vector<double> run(std::vector<double> inputs);
    std::vector<double> results();
    size_t inputSize() const;
    size_t outputSize() const;
    size_t numberOfLayers() const;
    inline std::vector<models::Layer> &layers() { return  _layers; }
    inline std::vector<models::Layer> const &layers() const { return  _layers; }
    void setBias(size_t layer, size_t neuron, double value);
    void setBiases(size_t layer, std::vector<double> const &);
    void setWeight(size_t layer, size_t neuron, size_t nextLayerNeuron, double value);
    void setWeigths(size_t layer, models::Matrix const &);
    std::string name() const;
    void setName(const std::string &name);
private:
    std::string _name;
    std::vector<models::Layer> _layers;
};

}
}

#endif // NET_H
