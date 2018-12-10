#include "Net.h"

#include <algorithm>
#include <iterator>

namespace mimir {
namespace models {

Net::Net(long inputs, long outputs)
{
    for (auto n = 0; n < inputs; ++n) {
        _input.addNeuron({});
    }
    for (auto n = 0; n < outputs; ++n) {
        _output.addNeuron({});
    }
}

void Net::addHiddenLayer(int numNeurons)
{
    if (_input.isConnected()) {
        throw std::logic_error("can't add layers after net is connected.");
    }
    Layer l;
    for (auto n = 0; n < numNeurons; ++n) {
        l.addNeuron({});
    }
    _hiddenLayers.push_back(l);
}

void Net::connect()
{
    if (_input.isConnected()) {
        return;
    }
    if (_hiddenLayers.size() == 0) {
        _input.connect(_output);
        return;
    }
    auto previous = _hiddenLayers.begin();
    auto next = previous;
    ++next;
    _input.connect(*previous);
    while (next != _hiddenLayers.end()) {
        (*previous).connect(*(next));
        ++previous; ++next;
    }
    (previous)->connect(_output);
}

std::valarray<double> Net::run(std::vector<double> inputs)
{
    _input.reset();
    _input.addInput(inputs);
    _input.run();
    std::for_each(_hiddenLayers.begin(), _hiddenLayers.end(),
                  [](Layer &layer){
        layer.run();
    });
    return _output.values();
}


std::valarray<double> Net::results()
{
    return _output.values();
}

void Net::backPropagate(const std::vector<double> &delta)
{

}

}
}
