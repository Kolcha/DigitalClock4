#-------------------------------------------------
#
# Project created by QtCreator 2013-08-13T20:52:25
#
#-------------------------------------------------

QT       += core gui widgets multimedia

include(../common.pri)

TARGET = alarm

SOURCES += alarm.cpp \
    gui/settings_dialog.cpp

HEADERS += alarm.h \
    alarm_settings.h \
    gui/settings_dialog.h

FORMS += \
    gui/settings_dialog.ui

TRANSLATIONS += \
    alarm_ru.ts

include(../../qm_gen.pri)

RESOURCES += alarm.qrc

OTHER_FILES += alarm.json

win32:RC_FILE = alarm.rc
