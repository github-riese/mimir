#ifndef BATCHITEM_H
#define BATCHITEM_H

#include <vector>

namespace mimir {
namespace models {

class BatchItem
{
public:
    BatchItem() = default;
    BatchItem(std::vector<double> const &input, std::vector<double> const &expectation);

    std::vector<double> const &input() const;
    void setInput(const std::vector<double> &input);

    std::vector<double> const &expectation() const;
    void setExpectation(const std::vector<double> &expectation);

private:
    std::vector<double> _input;
    std::vector<double> _expectation;
};

} // namespace models
} // namespace mimir

#endif // BATCHITEM_H
