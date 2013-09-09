#-------------------------------------------------
#
# Project created by QtCreator 2013-08-13T20:52:25
#
#-------------------------------------------------

QT       += core gui widgets multimedia
INCLUDEPATH += ../../digital_clock/core \
    ..

TARGET = alarm
TEMPLATE = lib
CONFIG += plugin

SOURCES += alarm.cpp \
    gui/settings_dlg.cpp

HEADERS += alarm.h \
    ../../digital_clock/core/iclock_plugin.h \
    ../plugin_settings.h \
    gui/settings_dlg.h \
    alarm_settings.h
OTHER_FILES += alarm.json

win32:RC_FILE = alarm.rc

FORMS += \
    gui/settings_dlg.ui

RESOURCES += \
    alarm.qrc
