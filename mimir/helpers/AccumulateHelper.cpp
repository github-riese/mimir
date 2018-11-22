//
// Created by riese on 2018-10-16.
//

#include "AccumulateHelper.h"

using std::vector;

using mimir::models::ColumnIndexValuePair;
using mimir::models::CPT;
using mimir::models::Probability;

namespace mimir::helpers {
    Probability AccumulateHelper::operator()(Probability p, CPT::Row const &row)
    {
        if (!row.matchesInput(input)) {
            return p;
        }
        return p + row.probability;
    }

}