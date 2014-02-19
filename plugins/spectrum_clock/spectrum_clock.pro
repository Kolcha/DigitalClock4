#-------------------------------------------------
#
# Project created by QtCreator 2013-07-17T19:57:53
#
#-------------------------------------------------

QT       += core

TARGET = spectrum_clock

SOURCES += spectrum_clock.cpp

HEADERS += spectrum_clock.h
OTHER_FILES += spectrum_clock.json

TRANSLATIONS += \
    spectrum_clock_ru.ts

win32:RC_FILE = spectrum_clock.rc

RESOURCES += \
    spectrum_clock.qrc

include(../common.pri)
