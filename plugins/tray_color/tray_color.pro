#-------------------------------------------------
#
# Project created by QtCreator 2013-10-20T11:43:12
#
#-------------------------------------------------

QT       += core gui widgets
INCLUDEPATH += ../../digital_clock/core \
    ..

TARGET = tray_color
TEMPLATE = lib
CONFIG += plugin

SOURCES += tray_color.cpp

HEADERS += tray_color.h \
    ../../digital_clock/core/iclock_plugin.h \
    ../plugin_settings.h \
    tray_color_settings.h
OTHER_FILES += tray_color.json

win32:RC_FILE = tray_color.rc
