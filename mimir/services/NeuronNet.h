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
    NeuronNet(size_t inputs, size_t outputs, const std::string &);
    void addHiddenLayer(size_t numNeurons, const std::string & = std::string());
    void appendLayer(size_t numNeurons, const std::string & = std::string());
    void connect();
    std::vector<double> run(std::vector<double> inputs);
    std::vector<double> results();
    size_t sizeOfLayer(size_t layer) const;
    size_t numberOfLayers() const;
    bool addNodes(size_t layer, size_t count);
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
