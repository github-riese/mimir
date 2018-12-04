#include "Net.h"

#include <algorithm>
#include <iterator>

namespace mimir {
namespace models {

Net::Net(long inputs, long outputs)
{
    for (auto n = 0; n < inputs; ++n) {
        _input.addNeuron(std::make_shared<Neuron>());
    }
    for (auto n = 0; n < outputs; ++n) {
        _output.addNeuron(std::make_shared<Neuron>());
    }
}

void Net::addHiddenLayer(int numNeurons)
{
    if (_input.isConnected()) {
        throw std::logic_error("can't add layers after net is connected.");
    }
    Layer l;
    for (auto n = 0; n < numNeurons; ++n) {
        l.addNeuron(std::make_shared<Neuron>());
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
        (previous)->connect(*(next));
        ++previous; ++next;
    }
    (previous)->connect(_output);
}

std::vector<double> Net::run(std::vector<double> inputs)
{
    _input.reset();
    _input.setValues(inputs);
    _input.run();
    std::for_each(_hiddenLayers.begin(), _hiddenLayers.end(),
                  [](Layer &layer){
        layer.run();
    });
    return _output.values();
}


std::vector<double> Net::results()
{
    return _output.values();
}

}
}
