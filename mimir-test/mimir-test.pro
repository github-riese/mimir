#-------------------------------------------------
#
# Project created by QtCreator 2018-05-24T23:32:08
#
#-------------------------------------------------

QT       += testlib

#QT       -= gui

TARGET = mimir-test
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += c++1z

TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        tst_Models.cpp \
    test-runner.cpp \
    TestListener.cpp \
    TestRunner.cpp \
    TestPatternFind.cpp \
    TestHelpers.cpp \
    TestMatrix.cpp \
    TestTrainer.cpp \
    TestSerialize.cpp

HEADERS += \
    tst_Models.h \
    TestListener.h \
    TestRunner.h \
    TestPatternFind.h \
    TestHelpers.h \
    TestMatrix.h \
    TestTrainer.h \
    TestSerialize.h

unix|win32: LIBS += -L$$OUT_PWD/../mimir/ -lmimir

INCLUDEPATH += $$PWD/../mimir
DEPENDPATH += $$PWD/../mimir

unix|win32: LIBS += -L$$OUT_PWD/../iotaomegapsi/tools/ -ltools

INCLUDEPATH += $$PWD/..
DEPENDPATH += $$PWD/../iotaomegapsi/tools
