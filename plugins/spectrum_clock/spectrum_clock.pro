#-------------------------------------------------
#
# Project created by QtCreator 2013-07-17T19:57:53
#
#-------------------------------------------------

QT       += core
INCLUDEPATH += ../../digital_clock/core

TARGET = spectrum_clock
TEMPLATE = lib
CONFIG += plugin

SOURCES += spectrum_clock.cpp

HEADERS += spectrum_clock.h \
    ../../digital_clock/core/iclock_plugin.h
OTHER_FILES += spectrum_clock.json

win32:RC_FILE = spectrum_clock.rc
