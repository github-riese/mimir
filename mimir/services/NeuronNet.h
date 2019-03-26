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
    bool addNode(size_t layer, double bias = 0., std::vector<double> weightsIn = {}, std::valarray<double> weightsOut = {});
    inline std::vector<models::Layer> &layers() { return  _layers; }
    inline std::vector<models::Layer> const &layers() const { return  _layers; }
    models::Layer const &layer(size_t n) const;
    models::Layer &layer(size_t n);
    void setBias(size_t layer, size_t neuron, double value);
    void setBiases(size_t layer, std::vector<double> const &);
    void setWeight(size_t layer, size_t neuron, size_t nextLayerNeuron, double value);
    void setWeigths(size_t layer, models::Matrix const &);
    std::string name() const;
    void setName(const std::string &name);
    double loss(std::vector<std::vector<double>> const &hypotheses, std::vector<std::vector<double>> const &expectations) const;
private:
    std::string _name;
    std::vector<models::Layer> _layers;
};

}
}

#endif // NET_H
