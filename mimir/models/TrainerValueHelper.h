#ifndef TRAINERVALUEHELPER_H
#define TRAINERVALUEHELPER_H

#include <vector>

namespace mimir {
namespace models {

class TrainerValueHelper
{
public:
    TrainerValueHelper(std::vector<double> const &hypothesis, std::vector<double> const &expectation);
    const std::vector<double> &hypothesis() const;
    void setHypothesis(const std::vector<double> &hypothesis);

    const std::vector<double> &expectation() const;
    void setExpectation(const std::vector<double> &expectation);
    inline double size() const noexcept { return  _hypothesis.size(); }
private:
    std::vector<double> _hypothesis;
    std::vector<double> _expectation;
};

} // namespace models
} // namespace mimir

#endif // TRAINERVALUEHELPER_H
