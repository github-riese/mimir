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
        std::vector<std::vector<double>> deltas;
        void addDelta(std::vector<double> const&);
        std::vector<double> averageDeltas() const;
    };
    std::deque<Batch> _batches;
    double _currentError = -1;
};

} // namespace services
} // namespace mimir

#endif // TRAINER_H
