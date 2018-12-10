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

void Neuron::resetInput()
{
    _input = 0.;
    _dirty = true;
}

void Neuron::setBias(double bias)
{
    _bias = bias;
    _dirty = true;
}

double Neuron::bias() const
{
    return _bias;
}

void Neuron::addInput(double value)
{
    _input += value;
    _dirty = true;
}

double Neuron::input() const
{
    return _input;
}

double Neuron::value()
{
    if (_dirty){
        _value = activate();
        _dirty = false;
    }
    return _value;
}

double Neuron::deriviateActivation() const
{
    return derivativeActivate();
}

bool Neuron::operator==(const Neuron &rhs) const
{
    return _id == rhs._id;
}

Neuron &Neuron::operator <<(double value)
{
    addInput(value);
    return *this;
}

double Neuron::activate() const
{
    return std::tanh(_input + _bias);
}

double Neuron::derivativeActivate() const
{
    return 1/std::pow(std::cosh(_input + _bias), 2.);
}

mimir::models::Neuron::operator double()
{
    return value();
}

} // namespace models
} // namespace mimir
