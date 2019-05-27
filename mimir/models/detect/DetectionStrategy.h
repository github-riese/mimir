#ifndef DETECTIONSTRATEGY_H
#define DETECTIONSTRATEGY_H

namespace mimir {
namespace models {
namespace detect {

enum DetectStrategy : int
{
    MaximizeOutcome,
    Canonical
};

} // namespace detect
} // namespace models
} // namespace mimir
#endif // DETECTIONSTRATEGY_H
