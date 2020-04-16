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
    using MinibatchResultCallback = std::function<void(float currentError, float detectRate, unsigned epochsNeeded)>;
    Trainer(NeuronNet &net);
    void addTrainingData(std::vector<float> input, std::vector<float> expectation);
    unsigned run(size_t batchSize, unsigned maxEpochs, float maxError, float minRate, float eta, MinibatchResultCallback resultCallback = nullptr);
    float currentError() const;
    void createGradients();
    void resetBatches();
private:
    std::tuple<unsigned, float, float> runMinibatch(std::vector<models::BatchItem> &, unsigned maxEpochs, float maxError, float minRate, float eta);
    void resetGradients();
    bool detectedCorrectly(std::vector<float> const &,std::vector<float> const &, float maxError = 0.) const;
    void calculateGradients(const std::vector<float> &expectation);
    void applyGradient(float eta);
    float detectRate(std::vector<std::vector<float>> const &results, std::vector<std::vector<float>> const &expectations);
private:
    NeuronNet &_net;
    MinibatchResultCallback _callback = nullptr;
    std::vector<std::vector<float>> _biasGradient;
    std::vector<models::Matrix> _weightGradient;
    std::vector<models::BatchItem> _batch;
    float _currentError;
};

} // namespace neuronnet
} // namespace services
} // namespace mimir

#endif // MIMIR_TRAINER_H
