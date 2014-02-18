#-------------------------------------------------
#
# Project created by QtCreator 2013-12-26T22:00:43
#
#-------------------------------------------------

include(../../config.pri)
include(../common.pri)

QT       += core gui widgets

TARGET = power_off

SOURCES += power_off.cpp \
    gui/settings_dialog.cpp

HEADERS += power_off.h \
    gui/settings_dialog.h \
    power_off_settings.h
OTHER_FILES += power_off.json

win32:RC_FILE = power_off.rc

RESOURCES += \
    power_off.qrc

TRANSLATIONS += \
    power_off_ru.ts

FORMS += \
    gui/settings_dialog.ui

win32:LIBS += -lAdvapi32
