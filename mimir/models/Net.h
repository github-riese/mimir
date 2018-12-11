#ifndef NET_H
#define NET_H

#include "models/Layer.h"

#include <valarray>
#include <vector>

namespace mimir {
namespace models {

class Net
{
public:
    Net(long inputs, long outputs);
    void addHiddenLayer(int numNeurons);
    void connect();
    std::valarray<double> run(std::vector<double> inputs);
    std::valarray<double> results();
    void backPropagate(const std::valarray<double> &expectation);
private:
    std::vector<Layer> _layers;
    Layer _output;
};

}
}

#endif // NET_H
