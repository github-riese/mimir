TEMPLATE = lib
QT -= gui

CONFIG += c++1z library

CONFIG(release, debug|release) {
mac: QMAKE_CXXFLAGS += -O3 -msse4.1
}

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
    helpers/helpers.cpp \
    helpers/math.cpp \
    helpers/AccumulateHelper.cpp \
    models/CPT.cpp \
    models/Evaluation.cpp \
    models/KeyValuePair.cpp \
    models/Probability.cpp \
    models/ProbabilityDistribution.cpp \
    models/ValueIndex.cpp \
    models/Layer.cpp \
    models/Matrix.cpp \
    models/activation/Sigmoid.cpp \
    models/activation/RectifiedLinear.cpp \
    models/activation/Softmax.cpp \
    services/neuronnet/ActivationsManager.cpp \
    services/DataStore.cpp \
    services/DependencyDetector.cpp \
    services/Evaluator.cpp \
    services/NameResolver.cpp \
    services/NameResolveSerializer.cpp \
    services/neuronnet/NeuronNet.cpp \
    services/neuronnet/NeuronNetSerializer.cpp \
    services/neuronnet/Trainer.cpp \
    models/activation/AbstractMSELossActivator.cpp \
    models/neuronnet/BatchItem.cpp \
    models/neuronnet/TrainerValueHelper.cpp \
    models/BayesNetFragment.cpp

HEADERS += \
    helpers/AccumulateHelper.h \
    helpers/helpers.h \
    models/CPT.h \
    models/Evaluation.h \
    models/KeyValuePair.h \
    models/NamedProbability.h \
    models/Probability.h \
    models/ProbabilityDistribution.h \
    models/ProbabilityWithPriors.h \
    models/ValueIndex.h \
    services/DataStore.h \
    services/DependencyDetector.h \
    services/Evaluator.h \
    services/NameResolver.h \
    traits/Timing.h \
    models/Layer.h \
    helpers/math.h \
    models/Matrix.h \
    services/neuronnet/NeuronNet.h \
    services/neuronnet/Trainer.h \
    services/neuronnet/NeuronNetSerializer.h \
    services/neuronnet/ActivationsManager.h \
    services/NameResolveSerializer.h \
    models/activation/ActivationInterface.h \
    models/activation/Sigmoid.h \
    models/activation/RectifiedLinear.h \
    models/activation/Softmax.h \
    models/activation/AbstractMSELossActivator.h \
    models/neuronnet/BatchItem.h \
    models/neuronnet/TrainerValueHelper.h \
    models/BayesNetFragment.h

unix|win32: LIBS += -lcuda -L/usr/local/cuda/lib
