#-------------------------------------------------
#
# Project created by QtCreator 2019-05-14T13:07:15
#
#-------------------------------------------------

QT       -= core gui

CONFIG   += c++1z
TARGET = tools
TEMPLATE = lib
CONFIG += staticlib

INCLUDEPATH += $$PWD/../..
DEFINES += TOOLS_LIBRARY

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
        logger/SinkManager.cpp \
        logger/ConsoleLogSink.cpp \
        logger/DebugLogSink.cpp \
        logger/LogMessage.cpp \
        logger/Logger.cpp \
        logger/OstreamLogSink.cpp

HEADERS += \
        logger/AbstractLogsink.h \
        logger/ConsoleLogSink.h \
        logger/DebugLogSink.h \
        logger/LogLevel.h \
        logger/LogMessage.h \
        logger/Logger.h \
        logger/OstreamLogSink.h \
        logger/SinkManager.h \
        timer/Timing.h \
        ../iotaomegapsi_global.h 

unix {
    target.path = /usr/lib/iotaomegapsi
    INSTALLS += target
}
