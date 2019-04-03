#ifndef BATCHITEM_H
#define BATCHITEM_H

#include <vector>

namespace mimir {
namespace models {

class BatchItem
{
public:
    BatchItem() = default;
    BatchItem(std::vector<double> const &input, std::vector<double> const &expectation, std::vector<double> const &hypothesis);

    std::vector<double> const &input() const;
    void setInput(const std::vector<double> &input);

    std::vector<double> const &expectation() const;
    void setExpectation(const std::vector<double> &expectation);

    std::vector<double> const &hypothesis() const;
    void setHypothesis(const std::vector<double> &hypothesis);

private:
    std::vector<double> _input;
    std::vector<double> _expectation;
    std::vector<double> _hypothesis;
};

} // namespace models
} // namespace mimir

#endif // BATCHITEM_H
