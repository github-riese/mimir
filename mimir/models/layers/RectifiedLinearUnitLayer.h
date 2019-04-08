#ifndef RECTIFIEDLINEARUNITLAYER_H
#define RECTIFIEDLINEARUNITLAYER_H

#include <mimir/models/layers/AbstractLayer.h>

namespace mimir {
namespace models {
namespace layers {

class RectifiedLinearUnitLayer : public AbstractLayer
{
public:
    RectifiedLinearUnitLayer();

    // AbstractLayer interface
    std::string name() const noexcept override;
    std::vector<double> gradientOnLoss(const std::vector<double> &expectation, const AbstractLayer *previousLayer) noexcept override;
    std::vector<double> gradient(const AbstractLayer *previous) noexcept override;
protected:
    void activate() noexcept override;
private:
    std::vector<double> derivative() const noexcept;
};

} // namespace layers
} // namespace models
} // namespace mimir

#endif // RECTIFIEDLINEARUNITLAYER_H
