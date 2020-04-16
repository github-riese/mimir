#ifndef BATCHITEM_H
#define BATCHITEM_H

#include <vector>

namespace mimir {
namespace models {

class BatchItem
{
public:
    BatchItem() = default;
    BatchItem(std::vector<float> const &input, std::vector<float> const &expectation);

    std::vector<float> const &input() const;
    void setInput(const std::vector<float> &input);

    std::vector<float> const &expectation() const;
    void setExpectation(const std::vector<float> &expectation);

private:
    std::vector<float> _input;
    std::vector<float> _expectation;
};

} // namespace models
} // namespace mimir

#endif // BATCHITEM_H
