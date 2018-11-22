//
// Created by riese on 2018-10-16.
//

#ifndef MIMIR_ACCUMULATEHELPER_H
#define MIMIR_ACCUMULATEHELPER_H

#include <vector>

#include "../models/KeyValuePair.h"
#include "../models/Probability.h"
#include "../models/CPT.h"

namespace mimir::helpers {
    struct AccumulateHelper
    {
        std::vector<models::ColumnIndexValuePair> input;
        models::Probability operator()(models::Probability p, models::CPT::Row const& row);
    };
}

#endif //MIMIR_ACCUMULATEHELPER_H
