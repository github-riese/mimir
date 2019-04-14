#ifndef ACTIVATIONSMANAGER_H
#define ACTIVATIONSMANAGER_H

#include <memory>
#include <vector>

#include "models/activation/ActivationInterface.h"

namespace mimir {
namespace services {
namespace neuronnet {

class ActivationsManager
{
public:
    ActivationsManager();
    template<typename Activation>
    size_t registerActivation(Activation *a = nullptr)
    {
        bool delAfterwards = false;
        if (a == nullptr) {
            a = new Activation;
            delAfterwards = true;
        }
        size_t existing = indexOf(a);
        if (delAfterwards) {
            delete a;
        }
        if (-1ul != existing) {
            return existing;
        }
        _knownActivations.push_back(std::make_shared<Activation>());
        return _knownActivations.size() - 1;
    }
    size_t indexOf(models::activation::ActivationInterface *) const;
    size_t indexOf(std::string const&) const;
    models::activation::ActivationInterface *get(size_t) const;
    models::activation::ActivationInterface *get(std::string const &) const;
private:
    std::vector<std::shared_ptr<models::activation::ActivationInterface>> _knownActivations;
};

ActivationsManager &getActivationsManager();
} // namespace neuronnet
} // namespace services
} // namespace mimir

#endif // ACTIVATIONSMANAGER_H
