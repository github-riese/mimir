#include "Neuron.h"

#include <cmath>
#include <numeric>

namespace mimir {
namespace models {

std::atomic<int> Neuron::__neuronIdSource = 0;
Neuron::Neuron() :
    _id(++__neuronIdSource)
{

}

int Neuron::id() const
{
    return _id;
}

void Neuron::reset()
{
    _input.clear();
    _bias = .0;
}

void Neuron::setBias(double bias)
{
    _bias = bias;
}

double Neuron::bias() const
{
    return _bias;
}

void Neuron::addInput(double value)
{
    _input.push_back(value);
}

double Neuron::value() const
{
    auto sum = std::accumulate(_input.begin(), _input.end(), 0.) + _bias;
    return std::tanh(sum);
}

bool Neuron::operator==(const Neuron &rhs) const
{
    return _id == rhs._id;
}

bool Neuron::operator==(const std::shared_ptr<Neuron> &rhs) const
{
    return _id == rhs->_id;
}

mimir::models::Neuron::operator double() const
{
    return value();
}

} // namespace models
} // namespace mimir
