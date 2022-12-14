#ifndef NET_H
#define NET_H

#include <mimir/models/neuronnet/BatchItem.h>
#include <mimir/models/Layer.h>
#include <mimir/models/Matrix.h>

#include <valarray>
#include <vector>
#include <string>

namespace mimir {
namespace services {
namespace neuronnet {

class NeuronNet
{
public:
    NeuronNet();
    NeuronNet(size_t inputs, size_t outputs, const std::string &);
    void addHiddenLayer(size_t numNeurons, const std::string & = std::string());
    void appendLayer(size_t numNeurons, const std::string & = std::string());
    void connect();
    std::vector<float> run(std::vector<float> inputs);
    std::vector<float> results();
    size_t sizeOfLayer(size_t layer) const;
    size_t numberOfLayers() const;
    bool addNode(size_t layer, float bias = 0., std::vector<float> weightsIn = {}, std::valarray<float> weightsOut = {});
    inline std::vector<models::Layer> &layers() { return  _layers; }
    inline std::vector<models::Layer> const &layers() const { return  _layers; }
    models::Layer const &layer(size_t n) const;
    models::Layer &layer(size_t n);
    void setBias(size_t layer, size_t neuron, float value);
    void setBiases(size_t layer, std::vector<float> const &);
    void setWeight(size_t layer, size_t neuron, size_t nextLayerNeuron, float value);
    void setWeigths(size_t layer, models::Matrix const &);
    std::string name() const;
    void setName(const std::string &name);
    float error(const std::vector<float> &expectation) const;
private:
    std::string _name;
    std::vector<models::Layer> _layers;
};

}
}
}

#endif // NET_H
