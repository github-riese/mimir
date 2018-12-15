#include "Trainer.h"

#include "helpers/helpers.h"

#include <iostream>

namespace mimir {
namespace services {

template <typename T>
std::vector<T> operator -(std::vector<T> const &left, std::vector<T> const &right)
{
    std::vector<T> result(left.size());
    std::transform(left.begin(), left.end(), right.begin(), result.begin(), [](T const &l, T const &r) -> T{ return l - r;});
    return result;
}

Trainer::Trainer(NeuronNet &net) :
    _net(net)
{
}

void Trainer::addBatch(std::vector<double> input, std::vector<double> expectation)
{
    if (expectation.size() != _net.outputSize()) {
        throw new std::logic_error("Expectation set size must be equal to the size of the output layer.");
    }
    _batches.push_back({input, expectation});
}

unsigned Trainer::run(unsigned maxEpochs, double maxError, double eta)
{
    std::cout.setf(std::ios::fixed, std::ios::floatfield);
    std::cout.precision(20);
    std::vector<double> differences(_net.outputSize());
    for (unsigned epoch = 0; epoch < maxEpochs; ++epoch) {
        Epoch epochData;
        for(auto item : _batches) {
            auto result = _net.run(item.input);
            epochData.addResult(result, item.expectedResult);
        }
        _currentError = std::accumulate(epochData.mses.begin(), epochData.mses.end(), 0., [](double current, double v) -> double { return current + std::pow(v, 2); })/static_cast<double>(_batches.size());
        if (epoch % 100 == 0) {
            std::cout << "Epoch " << epoch << ", mse: " << _currentError << ", eta: " << eta << "\n";
        }
        if (_currentError <= maxError) {
            std::cout << "Goal of error <= " << maxError << " reached after " << epoch << " epochs.\n";
            return epoch;
        }
        eta -= (epoch * .00000001) * eta;
        _net.backPropagate(epochData.results, epochData.expectations, eta);
    }
    std::cout << "Error level not reached.\n";
    return maxEpochs;
}

double Trainer::currentError() const
{
    return _currentError;
}

void Trainer::reset()
{
    _batches.clear();
}

void Trainer::Epoch::addResult(const std::vector<double> &result, const std::vector<double> &expected)
{
    results.push_back(result);
    expectations.push_back(expected);
    std::vector<double> delta = result - expected;
    deltas.push_back(delta);
    mses.push_back(std::accumulate(delta.begin(), delta.end(), 0.));
}

} // namespace services
} // namespace mimir
