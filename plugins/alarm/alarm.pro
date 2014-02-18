#-------------------------------------------------
#
# Project created by QtCreator 2013-08-13T20:52:25
#
#-------------------------------------------------

include(../../config.pri)
include(../common.pri)

QT       += core gui widgets multimedia

TARGET = alarm

SOURCES += alarm.cpp \
    gui/settings_dialog.cpp

HEADERS += alarm.h \
    alarm_settings.h \
    gui/settings_dialog.h
OTHER_FILES += alarm.json

win32:RC_FILE = alarm.rc

FORMS += \
    gui/settings_dialog.ui

RESOURCES += \
    alarm.qrc

TRANSLATIONS += \
    alarm_ru.ts
