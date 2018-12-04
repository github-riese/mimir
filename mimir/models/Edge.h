#ifndef EDGE_H
#define EDGE_H

#include <memory>
#include <vector>
#include "models/Neuron.h"

namespace mimir {
namespace models {

class Edge
{
public:
    Edge(const std::shared_ptr<Neuron> &input, const std::shared_ptr<Neuron> &output, double weight);

    double weight() const;

    void setWeight(double weight);

    void propagate();
private:
    std::shared_ptr<Neuron> _input;
    std::shared_ptr<Neuron> _output;
    double _weight;
};

} // namespace models
} // namespace mimir

#endif // EDGE_H
