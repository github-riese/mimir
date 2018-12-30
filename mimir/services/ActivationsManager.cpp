#include "ActivationsManager.h"
#include <typeinfo>
#include <typeindex>

namespace mimir {
namespace services {

ActivationsManager::ActivationsManager()
{
    registerActivation<helpers::Sigmoid>();
    registerActivation<helpers::RectifiedLinear>();
}

size_t ActivationsManager::indexOf(helpers::Activation *a) const
{
    auto found = std::find_if(_knownActivations.begin(),
                              _knownActivations.end(),
                              [&a](std::shared_ptr<helpers::Activation> const& known) {
            if (a == nullptr) {
                return false;
            }
            auto x = std::type_index(typeid(*a));
            auto pure = known.get();
            auto y = std::type_index(typeid (*pure));
            return  x == y;
    });
    if (found == _knownActivations.end()) {
        return -1ul;
    }
    return static_cast<size_t>(std::distance(_knownActivations.begin(), found));
}

helpers::Activation *ActivationsManager::get(size_t index) const
{
    if (index >= _knownActivations.size()) {
        throw std::out_of_range("given activation indes is out of range");
    }
    return _knownActivations.at(index).get();
}

} // namespace services
} // namespace mimir
