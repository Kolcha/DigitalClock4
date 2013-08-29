#-------------------------------------------------
#
# Project created by QtCreator 2013-08-13T20:52:25
#
#-------------------------------------------------

QT       += core gui widgets
INCLUDEPATH += ../../core

TARGET = alarm_timer
TEMPLATE = lib
CONFIG += plugin

SOURCES += src/manager.cpp \
    gui/settings_dlg.cpp

HEADERS += src/manager.h \
    ../../core/iclock_plugin.h \
    gui/settings_dlg.h
OTHER_FILES += alarm_timer.json

win32:RC_FILE = alarm_timer.rc

FORMS += \
    gui/settings_dlg.ui

RESOURCES += \
    alarm_timer.qrc
