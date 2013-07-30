#-------------------------------------------------
#
# Project created by QtCreator 2013-07-30T21:02:55
#
#-------------------------------------------------

QT       += core gui
INCLUDEPATH += ../../core

TARGET = hour_signal
TEMPLATE = lib
CONFIG += plugin

SOURCES += hour_signal.cpp

HEADERS += hour_signal.h \
    ../../core/iclock_plugin.h
OTHER_FILES += hour_signal.json

win32:RC_FILE = hour_signal.rc

RESOURCES += \
    hour_signal.qrc
