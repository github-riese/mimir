#include "Evaluation.h"

#include <algorithm>
#include <numeric>

using std::pair;
using std::vector;

using std::max_element;

namespace mimir {
namespace models {

Evaluation::Evaluation()
{

}

void Evaluation::addProbability(ValueIndex classification, const Probability &probability)
{
    _probabilities[classification] = probability;
    auto max = max_element(_probabilities.begin(), _probabilities.end(), [](const pair<ValueIndex, Probability> &left, const pair<ValueIndex, Probability> &right){
        return left.second < right.second;
    });
    _mostProbable = (*max).first;
}

std::vector<ValueIndex> Evaluation::classifications() const
{
    vector<ValueIndex> classifications;
    for (auto element : _probabilities) {
        classifications.push_back(element.first);
    }
    return  classifications;
}

} // namespace models
} // namespace mimir
