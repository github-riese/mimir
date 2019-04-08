#ifndef SIGMOIDLAYER_H
#define SIGMOIDLAYER_H

#include <mimir/models/layers/AbstractLayer.h>

namespace mimir {
namespace models {
namespace layers {

class SigmoidLayer : public AbstractLayer
{
public:
    SigmoidLayer();

    // AbstractLayer interface
    std::string name() const noexcept override;
    std::vector<double> gradientOnLoss(std::vector<double> const &expectation, AbstractLayer const *previousLayer) noexcept override;
    std::vector<double> gradient(AbstractLayer const *previous) noexcept override;
protected:
    void activate() noexcept override;
private:
    std::vector<double> derivative() const noexcept;
};

} // namespace layers
} // namespace models
} // namespace mimir

#endif // SIGMOIDLAYER_H
