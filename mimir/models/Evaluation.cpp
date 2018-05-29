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
    long double sum = 0;
    for (auto p : _probabilities) {
        sum += p.second.probability();
    }
    _mostProbable = (*max).first;
    _average = sum / static_cast<long double>(_probabilities.size());
}

std::vector<ValueIndex> Evaluation::probableClassifications() const
{
    vector<ValueIndex> classifications;
    for (auto element : _probabilities) {
        classifications.push_back(element.first);
    }
    return  classifications;
}

} // namespace models
} // namespace mimir
