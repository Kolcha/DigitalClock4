#-------------------------------------------------
#
# Project created by QtCreator 2013-07-17T19:57:53
#
#-------------------------------------------------

QT       += core

include(../common.pri)

TARGET = spectrum_clock

SOURCES += spectrum_clock.cpp

HEADERS += spectrum_clock.h

TRANSLATIONS += \
    spectrum_clock_ru.ts

include(../../qm_gen.pri)

RESOURCES += spectrum_clock.qrc

OTHER_FILES += spectrum_clock.json

win32:RC_FILE = spectrum_clock.rc
