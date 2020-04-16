#ifndef TRAINERVALUEHELPER_H
#define TRAINERVALUEHELPER_H

#include <vector>

namespace mimir {
namespace models {

class TrainerValueHelper
{
public:
    TrainerValueHelper(std::vector<float> const &hypothesis, std::vector<float> const &expectation);
    const std::vector<float> &hypothesis() const;
    void setHypothesis(const std::vector<float> &hypothesis);

    const std::vector<float> &expectation() const;
    void setExpectation(const std::vector<float> &expectation);

private:
    std::vector<float> _hypothesis;
    std::vector<float> _expectation;
};

} // namespace models
} // namespace mimir

#endif // TRAINERVALUEHELPER_H
