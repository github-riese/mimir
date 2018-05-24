#include "Sampler.h"
#include <algorithm>

using std::find_if;
using std::make_pair;

namespace mimir {
namespace services {

Sampler::Sampler(std::string name) :
    _name(name)
{

}

void Sampler::addSample(models::Sample sample)
{
    _samples.add(sample);
}

} // namespace services
} // namespace mimir
