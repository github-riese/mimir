#ifndef ACTIVATIONSMANAGER_H
#define ACTIVATIONSMANAGER_H

#include <memory>
#include <vector>

#include "helpers/activations.h"

namespace mimir {
namespace services {

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
    size_t indexOf(helpers::Activation *) const;
    helpers::Activation *get(size_t) const;
private:
    std::vector<std::shared_ptr<helpers::Activation>> _knownActivations;
};

} // namespace services
} // namespace mimir

#endif // ACTIVATIONSMANAGER_H
