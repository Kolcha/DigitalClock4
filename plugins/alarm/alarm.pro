#-------------------------------------------------
#
# Project created by QtCreator 2013-08-13T20:52:25
#
#-------------------------------------------------

QT       += core gui widgets
INCLUDEPATH += ../../core

TARGET = alarm
TEMPLATE = lib
CONFIG += plugin

SOURCES += src/manager.cpp \
    gui/settings_dlg.cpp \
    alarm.cpp

HEADERS += src/manager.h \
    ../../core/iclock_plugin.h \
    gui/settings_dlg.h \
    alarm.h
OTHER_FILES += alarm.json

win32:RC_FILE = alarm.rc

FORMS += \
    gui/settings_dlg.ui

RESOURCES += \
    alarm.qrc
