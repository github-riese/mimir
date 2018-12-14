#ifndef NET_H
#define NET_H

#include "models/Layer.h"
#include "models/Matrix.h"

#include <valarray>
#include <vector>

namespace mimir {
namespace services {

class NeuronNet
{
public:
    NeuronNet(long inputs, long outputs);
    void addHiddenLayer(int numNeurons);
    void connect();
    std::vector<double> run(std::vector<double> inputs);
    std::vector<double> results();
    void backPropagate(const std::vector<std::vector<double> > &results, const std::vector<std::vector<double> > &expectations, double eta);
    size_t outputSize() const;
private:
    std::tuple<std::vector<std::vector<double>>, std::vector<models::Matrix>>
    deltaNabla(const std::vector<double> &costDerivative) const;
    std::vector<models::Matrix> addMatices(std::vector<models::Matrix> const &, std::vector<models::Matrix> const &) const;
    std::vector<std::vector<double>> addVectors(std::vector<std::vector<double>> const &, std::vector<std::vector<double>> const &) const;
private:
    std::vector<models::Layer> _layers;
    models::Layer _output;
};

}
}

#endif // NET_H
