#-------------------------------------------------
#
# Project created by QtCreator 2014-05-10T23:40:07
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = move_clock

SOURCES += move_clock.cpp \
    gui/settings_dialog.cpp

HEADERS += move_clock.h \
    gui/settings_dialog.h \
    move_clock_settings.h
OTHER_FILES += move_clock.json

win32::RC_FILE = move_clock.rc

FORMS += \
    gui/settings_dialog.ui

RESOURCES += \
    move_clock.qrc

TRANSLATIONS += \
    move_clock_ru.ts

include(../common.pri)
