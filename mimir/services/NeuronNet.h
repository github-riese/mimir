#ifndef NET_H
#define NET_H

#include "models/Layer.h"

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
    void backPropagate(const std::valarray<double> &expectation, double eta);
private:
    std::vector<models::Layer> _layers;
    models::Layer _output;
};

}
}

#endif // NET_H
