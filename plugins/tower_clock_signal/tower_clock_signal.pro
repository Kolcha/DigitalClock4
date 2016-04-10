#-------------------------------------------------
#
# Project created by QtCreator 2016-03-22T09:03:32
#
#-------------------------------------------------

QT       += core multimedia

include(../common.pri)

TARGET = tower_clock_signal

SOURCES += tower_clock_signal.cpp

HEADERS += tower_clock_signal.h

DISTFILES += tower_clock_signal.json

TRANSLATIONS += \
    tower_clock_signal_ru.ts

include(../../qm_gen.pri)

RESOURCES += tower_clock_signal.qrc

win32:RC_FILE = tower_clock_signal.rc
