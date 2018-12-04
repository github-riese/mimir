#include "Edge.h"

namespace mimir {
namespace models {

Edge::Edge(const std::shared_ptr<Neuron> &input, const std::shared_ptr<Neuron> &output, double weight) :
    _input(input),
    _output(output),
    _weight(weight)
{

}

double Edge::weight() const
{
    return _weight;
}

void Edge::setWeight(double weight)
{
    _weight = weight;
}

void Edge::propagate()
{
    _output->addInput(_input->value() * _weight);
}

} // namespace models
} // namespace mimir
