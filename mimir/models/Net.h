#ifndef NET_H
#define NET_H

#include "models/Layer.h"

#include <vector>

namespace mimir {
namespace models {

class Net
{
public:
    Net(long inputs, long outputs);
    void addHiddenLayer(int numNeurons);
    void connect();
    std::vector<double> run(std::vector<double> inputs);
    std::vector<double> results();
private:
    Layer _input;
    std::vector<Layer> _hiddenLayers;
    Layer _output;
};

}
}

#endif // NET_H
