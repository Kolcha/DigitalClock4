#-------------------------------------------------
#
# Project created by QtCreator 2013-10-21T22:29:49
#
#-------------------------------------------------

include(../config.pri)

QT       += core gui widgets

TARGET = clock_common
TEMPLATE = lib

DEFINES += CLOCK_COMMON_LIBRARY

SOURCES += \
    settings_keys.cpp \
    plugin_settings.cpp \
    centered_dialog.cpp \
    plugin_base.cpp

HEADERS += \
    settings_keys.h \
    iclock_plugin.h \
    plugin_settings.h \
    clock_common_global.h \
    centered_dialog.h \
    plugin_base.h

win32:RC_FILE = clock_common.rc
unix:VERSION = 1.0.1
