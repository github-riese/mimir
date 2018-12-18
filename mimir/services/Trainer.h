#ifndef TRAINER_H
#define TRAINER_H

#include <deque>

#include "services/NeuronNet.h"

namespace mimir {
namespace services {

class Trainer
{
public:
    Trainer(NeuronNet &net);
    void addBatch(std::vector<double> input, std::vector<double> expectation);
    unsigned run(unsigned maxEpochs, double maxError, double eta);
    double currentError() const;
    void reset();
private:
    NeuronNet &_net;
    struct Batch
    {
        std::vector<double> input;
        std::vector<double> expectedResult;
    };

    struct Epoch {
        void addResult(std::vector<double> const&result, std::vector<double> const &expected);
        int hitrate();
        std::vector<std::vector<double>> results;
        std::vector<std::vector<double>> expectations;
        std::vector<std::vector<double>> deltas;
        std::vector<double> mses;
    };
    std::vector<int> expectedSums;
    std::vector<int> detectedSums;
    std::deque<Batch> _batches;
    double _currentError = -1;
};

} // namespace services
} // namespace mimir

#endif // TRAINER_H
