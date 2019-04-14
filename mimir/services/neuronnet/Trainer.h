#ifndef MIMIR_TRAINER_H
#define MIMIR_TRAINER_H

#include <vector>

#include <mimir/models/neuronnet/BatchItem.h>
#include <mimir/models/neuronnet/TrainerValueHelper.h>
#include <mimir/models/Matrix.h>
#include "NeuronNet.h"


namespace mimir {
namespace services {
namespace neuronnet {

class Trainer
{
public:
    using MinibatchResultCallback = std::function<void(double currentError, double detectRate, unsigned epochsNeeded)>;
    Trainer(NeuronNet &net);
    void addTrainingData(std::vector<double> input, std::vector<double> expectation);
    unsigned run(size_t batchSize, unsigned maxEpochs, double maxError, double minRate, double eta, MinibatchResultCallback resultCallback = nullptr);
    double currentError() const;
    void createGradients();
    void resetBatches();
private:
    std::tuple<unsigned, double, double> runMinibatch(std::vector<models::BatchItem> &, unsigned maxEpochs, double maxError, double minRate, double eta);
    void resetGradients();
    bool detectedCorrectly(std::vector<double> const &,std::vector<double> const &, double maxError = 0.) const;
    void calculateGradients(const std::vector<double> &expectation);
    void applyGradient(double eta);
    double detectRate(std::vector<std::vector<double>> const &results, std::vector<std::vector<double>> const &expectations);
private:
    NeuronNet &_net;
    MinibatchResultCallback _callback = nullptr;
    std::vector<std::vector<double>> _biasGradient;
    std::vector<models::Matrix> _weightGradient;
    std::vector<models::BatchItem> _batch;
    double _currentError;
};

} // namespace neuronnet
} // namespace services
} // namespace mimir

#endif // MIMIR_TRAINER_H
