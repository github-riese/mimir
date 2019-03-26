#include "ActivationsManager.h"
#include <typeinfo>
#include <typeindex>

#include "models/activation/Sigmoid.h"
#include "models/activation/RectifiedLinear.h"
#include "models/activation/Softmax.h"

using namespace mimir::models::activation;

namespace mimir {
namespace services {

static ActivationsManager g_activationsManager;

ActivationsManager::ActivationsManager()
{
    registerActivation<Sigmoid>();
    registerActivation<RectifiedLinear>();
    registerActivation<Softmax>();
}

size_t ActivationsManager::indexOf(models::activation::ActivationInterface *a) const
{
    auto found = std::find_if(_knownActivations.begin(),
                              _knownActivations.end(),
                              [&a](std::shared_ptr<ActivationInterface> const& known) {
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

size_t ActivationsManager::indexOf(const std::string &name) const
{
    auto found = std::find_if(_knownActivations.begin(), _knownActivations.end(),
                              [name](std::shared_ptr<ActivationInterface> const &item) {
                                    return item->name() == name;
                                });
    if (found == _knownActivations.end()) {
        return -1ul;
    }
    return static_cast<size_t>(std::distance(_knownActivations.begin(), found));
}

models::activation::ActivationInterface *ActivationsManager::get(size_t index) const
{
    if (index >= _knownActivations.size()) {
        throw std::out_of_range("given activation indes is out of range");
    }
    return _knownActivations.at(index).get();
}

models::activation::ActivationInterface *ActivationsManager::get(const std::string &name) const
{
    size_t index = indexOf(name);
    if (-1ul == index) {
        return nullptr;
    }
    return get(index);
}

ActivationsManager &getActivationsManager()
{
    return g_activationsManager;
}

} // namespace services
} // namespace mimir
