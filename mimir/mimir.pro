TEMPLATE = lib
QT -= gui
QT += network

CONFIG += c++1y library

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    models/Probability.cpp \
    models/Sample.cpp \
    services/Sampler.cpp \
    models/SampleStore.cpp \
    models/ValueCounter.cpp \
    models/ValueIndex.cpp \
    services/NameResolver.cpp \
    models/Evaluation.cpp \
    services/Evaluator.cpp \
    services/SamplerManager.cpp \
    services/EvaluationCombiner.cpp \
    helpers/helpers.cpp

HEADERS += \
    models/Probability.h \
    models/Sample.h \
    services/Sampler.h \
    models/SampleStore.h \
    models/ValueCounter.h \
    models/ValueIndex.h \
    services/NameResolver.h \
    models/Evaluation.h \
    services/Evaluator.h \
    services/SamplerManager.h \
    models/ValueType.h \
    models/InterpolationStrategy.h \
    services/EvaluationCombiner.h \
    helpers/helpers.h \
    models/ProbabilityWithAPrioris.h
