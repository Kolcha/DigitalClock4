#-------------------------------------------------
#
# Project created by QtCreator 2013-10-21T22:29:49
#
#-------------------------------------------------

QT       += core gui

TARGET = clock_common
TEMPLATE = lib

DEFINES += CLOCK_COMMON_LIBRARY

SOURCES += \
    settings_keys.cpp \
    plugin_settings.cpp

HEADERS += \
    settings_keys.h \
    iclock_plugin.h \
    plugin_settings.h

win32:RC_FILE = clock_common.rc
