#include "Trainer.h"

#include "helpers/helpers.h"

#include <iostream>

namespace mimir {
namespace services {

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
        epochData.deltas.assign(_net.outputSize(), {});
        for(auto item : _batches) {
            auto result = _net.run(item.input);
            std::transform(result.begin(), result.end(), item.expectedResult.begin(), result.begin(), [](double result, double expect) -> double { return result - expect; });
//            _net.backPropagate(result, eta);
            epochData.addDelta(result);
        }
        auto avgDeltas = epochData.averageDeltas();
        _currentError = std::accumulate(avgDeltas.begin(), avgDeltas.end(), 0., [](double current, double v) -> double { return current + std::pow(v, 2); });
        if (epoch % 100 == 0) {
            std::cout << "Epoch " << epoch << ", mse: " << _currentError <<"\n";
        }
        if (_currentError <= maxError) {
            std::cout << "Goal of error <= " << maxError << " reached after " << epoch << " epochs.\n";
            return epoch;
        }
        _net.backPropagate(avgDeltas, eta);
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

void Trainer::Epoch::addDelta(const std::vector<double> &results)
{
    auto result = results.begin();
    auto delta = deltas.begin();
    while (result != results.end()) {
        (*delta).push_back(*result);
        ++result; ++delta;
    }
}

std::vector<double> Trainer::Epoch::averageDeltas() const
{
    std::vector<double> result;
    double sampleSize = static_cast<double>(deltas.front().size());
    for (auto row : deltas) {
        result.push_back(std::accumulate(row.begin(), row.end(), 0.)/sampleSize);
    }
    return result;
}


} // namespace services
} // namespace mimir
