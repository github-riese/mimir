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

template<typename T>
std::ostream &operator <<(std::ostream & stream, std::vector<T> const &data)
{
    auto precision = std::cout.precision(3);
    stream << "(";
    for(auto v : data) {
        stream << v << ' ';
    }
    stream << ")";
    std::cout.precision(precision);
    return stream;
}

unsigned Trainer::run(unsigned maxEpochs, double maxError, double eta)
{
    std::cout.setf(std::ios::fixed, std::ios::floatfield);
    std::cout.precision(10);
    std::vector<double> differences(_net.outputSize());
    double detectRate = 0.;
    double batchsize = static_cast<double>(_batches.size());
    size_t reportInterval = 10;
    detectedSums = std::vector<int>(_batches.front().expectedResult.size());
    expectedSums = std::vector<int>(detectedSums.size());
    for (unsigned epoch = 0; epoch < maxEpochs; ++epoch) {
        Epoch epochData;
        for(auto item : _batches) {
            auto result = _net.run(item.input);
            epochData.addResult(result, item.expectedResult);
            auto detectResult = std::max_element(result.begin(), result.end());
            auto detectExpect = std::max_element(item.expectedResult.begin(), item.expectedResult.end());
            auto dR = std::distance(result.begin(), detectResult);
            auto dE = std::distance(item.expectedResult.begin(), detectExpect);
            if (dR == dE) {
                ++detectedSums[static_cast<size_t>(dR)];
            }
            ++expectedSums[static_cast<size_t>(dE)];
        }
        _currentError = std::accumulate(epochData.mses.begin(), epochData.mses.end(), 0., [](double current, double v) -> double { return current + std::pow(v, 2); })/static_cast<double>(_batches.size());
        if (epoch % reportInterval == 0) {
            std::cout << "Epoch " << epoch << ", mse: " << _currentError;
            if (epoch > 0) {
                auto prec = std::cout.precision(2);
                std::cout << ", detectrate: " << detectRate/static_cast<double>(epoch%reportInterval+reportInterval)*100 << "%";
                std::cout << " (" << detectRate/static_cast<double>(epoch%reportInterval+reportInterval) *batchsize << "/" << _batches.size() << ") detect " ;
                std::cout << detectedSums;
                std::cout << "expect " << expectedSums;
                detectedSums.assign(detectedSums.size(), 0);
                expectedSums.assign(expectedSums.size(), 0);
                std::cout.precision(prec);
                detectRate = 0;
            }
            std::cout << std::endl;
            detectRate = 0.;
        }
        if (_currentError <= maxError) {
            std::cout << "Goal of error <= " << maxError << " reached after " << epoch << " epochs.\n";
            return epoch;
        }
        detectRate += (static_cast<double>(epochData.hitrate()))/batchsize;
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

int Trainer::Epoch::hitrate()
{
    int hits = 0;
    auto expect = expectations.begin();
    auto result = results.begin();
    while (expect != expectations.end()) {
        auto expectIdx = std::distance((*expect).begin(), std::max_element((*expect).begin(), (*expect).end()));
        auto resultIdx = std::distance((*result).begin(), std::max_element((*result).begin(), (*result).end()));
        if (expectIdx == resultIdx) {
            ++hits;
        }
        ++expect; ++result;
    }
    return hits;
}

} // namespace services
} // namespace mimir
