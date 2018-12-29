#ifndef TRAINER_H
#define TRAINER_H

#include <vector>

#include "models/Matrix.h"
#include "services/NeuronNet.h"


namespace mimir {
namespace services {

class Trainer
{
    struct BatchItem {
        std::vector<double> input;
        std::vector<double> expectation;
    };
public:
    using MinibatchResultCallback = std::function<void(double currentError, double detectRate, unsigned epochsNeeded)>;
    Trainer(NeuronNet &net);
    void addBatch(std::vector<double> input, std::vector<double> expectation);
    unsigned run(size_t batchSize, unsigned maxEpochs, double maxError, double eta, MinibatchResultCallback resultCallback = nullptr);
    double currentError() const;
private:
    void createGradients();
    void resetGradients();
    void backPropagate(std::vector<std::vector<double>> const &results, std::vector<std::vector<double>> const &expectations, double eta);
    std::tuple<unsigned, double, double> runMinibatch(std::vector<BatchItem> const &, unsigned maxEpochs, double maxError, double eta);
    double mse(std::vector<std::vector<double>> const &results, const std::vector<std::vector<double> > &miniBatch) const;
    double detectRate(std::vector<std::vector<double>> const &results, std::vector<std::vector<double>> const &expectations);
private:
    NeuronNet &_net;
    MinibatchResultCallback _callback = nullptr;
    std::vector<std::vector<double>> _biasGradient;
    std::vector<models::Matrix> _weightGradient;
    std::vector<BatchItem> _batch;
    double _currentError;
};

} // namespace services
} // namespace mimir

#endif // TRAINER_H
