#ifndef MAXCLASSTURNOUTDETECTOR_H
#define MAXCLASSTURNOUTDETECTOR_H

#include "services/detect/AbstractDependencyDetector.h"

namespace mimir {
namespace services {
namespace detect {

class MaxClassTurnOutDetector : public AbstractDependencyDetector
{
public:
    MaxClassTurnOutDetector(models::CPT &cpt, long classFieldIndex, models::ColumnIndexValuePairVector const &sample);
    /**
     * @brief Calculates direct class parents
     * Will return at most maxResults  vectors of key value pairs for which applies:
     * the vector length of the probabilit distribution as vector of the classified class given the parameters in the return value
     * is greater than the vector length of the pobability distribution of the classification without any dependend parameters
     * Or simply for each element in the result applies the classification will become more definite with the parameter as parent(s)
     * Or P(class|<element>) > P(class)
     * @param maxResults
     * @return
     */
    models::detect::InternalFragmentVector detect(models::ProbabilityDistribution const &) override;
private:
    std::vector<models::ColumnIndexValuePairVector> findDirectParents();
    void maximizeLikelyhoods();
    models::detect::InternalFragmentVector likelihoodFragments();
    models::detect::InternalNet buildNet(models::ColumnIndexValuePairVector const &parents, const models::detect::InternalFragmentVector &likelihoods);
    bool buildInternalNet(models::detect::InternalNet &baseNet, models::detect::InternalFragmentVector &level, models::detect::InternalFragment const &toBeAdded);
private:
    models::detect::InternalFragmentVector _likelihoods;
    models::ProbabilityDistribution _baseDistribution;
};

} // namespace detect
} // namespace services
} // namespace mimir

#endif // MAXCLASSTURNOUTDETECTOR_H
