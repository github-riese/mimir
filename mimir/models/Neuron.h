#ifndef NEURON_H
#define NEURON_H

#include <atomic>
#include <memory>
#include <vector>

namespace mimir {
namespace models {

class Neuron
{
public:
    Neuron();
    int id() const;

    void reset();

    void addInput(double value);
    double value() const;

    double bias() const;
    void setBias(double bias);

    bool operator==(const Neuron &rhs) const;
    bool operator==(const std::shared_ptr<Neuron>&rhs) const;

    operator double() const;
private:
    static std::atomic<int> __neuronIdSource;
    int _id;
    double _bias = 0.;
    std::vector<double> _input = {};
};

} // namespace models
} // namespace mimir

#endif // NEURON_H
