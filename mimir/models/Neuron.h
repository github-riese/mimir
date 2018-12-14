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

    void resetInput();

    void addInput(double value);
    double input() const;
    double value();
    double sigmoidPrime() const;

    double bias() const;
    void setBias(double bias);

    bool operator==(const Neuron &rhs) const;
    Neuron &operator << (double);

    operator double();

    double z() const;

protected:
    double activate();
    double derivativeActivate() const;
private:
    static std::atomic<int> __neuronIdSource;
    int _id;
    double _bias = 0.;
    double _input = 0.;
    bool _dirty = false;
    double _value = .0;
    double _z = 0.;
};

} // namespace models
} // namespace mimir

#endif // NEURON_H
