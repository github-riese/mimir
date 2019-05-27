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
    models::detect::InternalNetVector buildNets(models::ProbabilityDistribution const &, size_t maxResults) override;
private:
    models::detect::InternalNet buildNet(models::ColumnIndexValuePairVector const &parents, const models::detect::InternalFragmentVector &likelihoods);
    std::vector<models::ColumnIndexValuePairVector> findDirectParents(size_t maxResults);
    models::detect::InternalFragmentVector maximizeLikelyhoods();
private:
    struct FieldLikelihood { models::ColumnIndexValuePair field; models::ColumnIndexValuePairVector parents; models::Probability probability;};
    using FieldLikelihoodVector = std::vector<FieldLikelihood>;
    FieldLikelihoodVector _likelihoods;
    models::ProbabilityDistribution _baseDistribution;
};

} // namespace detect
} // namespace services
} // namespace mimir

#endif // MAXCLASSTURNOUTDETECTOR_H
