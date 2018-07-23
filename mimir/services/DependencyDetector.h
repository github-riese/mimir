#ifndef DEPENDENCYDETECTOR_H
#define DEPENDENCYDETECTOR_H

#include <utility>
#include <vector>

#include "../models/CPT.h"
#include "../models/ValueIndex.h"
#include "../models/KeyValuePair.h"
#include "../models/Network.h"
#include "../models/NetworkFragment.h"

namespace mimir {
namespace services {

class DependencyDetector
{
public:
    DependencyDetector(models::CPT &cpt);
    std::vector<models::NetworkFragment> computePriors(const std::vector<models::ColumnIndexValuePair> &input) const;
    models::Network findSuitableGraph(const std::vector<models::ColumnNameValuePair> &input);
private:
    std::vector<models::NetworkFragment> findLikelyGraphs(const std::vector<models::ColumnNameValuePair> &input) const;
    models::Network findBestGraphs(std::vector<models::NetworkFragment> const &candidates);
    models::Probability likelihood(models::ColumnIndexValuePair const &k, std::vector<models::ColumnIndexValuePair> const &input) const;
    models::Probability conditionalProbability(models::ColumnIndexValuePair const&, std::vector<models::ColumnIndexValuePair>const &);
    void eliminateZeroEvidence(std::vector<models::ColumnIndexValuePair> &) const;
    std::vector<models::ColumnNameValuePair> indexedPairVectorToNamedPairVector(std::vector<models::ColumnIndexValuePair> const &) const;
    std::vector<models::ColumnIndexValuePair> namedPairVectorToIndexedPairVector(std::vector<models::ColumnNameValuePair> const &) const;
private:
    models::CPT &_cpt;
};

} // namespace services
} // namespace mimir

#endif // DEPENDENCYDETECTOR_H
