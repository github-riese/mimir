project(mimir)
set(CMAKE_CXX_STANDARD 17)
add_library(mimir SHARED
        helpers/helpers.cpp
        helpers/helpers.h
        models/CPT.cpp
        models/CPT.h
        models/Evaluation.cpp
        models/Evaluation.h
        models/KeyValuePair.cpp
        models/KeyValuePair.h
        models/Network.cpp
        models/Network.h
        models/NetworkFragment.cpp
        models/NetworkFragment.h
        models/Probability.cpp
        models/Probability.h
        models/ProbabilityDistribution.cpp
        models/ProbabilityDistribution.h
        models/ProbabilityWithPriors.h
        models/ValueIndex.cpp
        models/ValueIndex.h
        services/DataStore.cpp
        services/DataStore.h
        services/DependencyDetector.cpp
        services/DependencyDetector.h
        services/Evaluator.cpp
        services/Evaluator.h
        services/NameResolver.cpp
        services/NameResolver.h
        traits/Timing.h mimir.cpp mimir.h helpers/AccumulateHelper.cpp helpers/AccumulateHelper.h)