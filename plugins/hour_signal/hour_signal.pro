#-------------------------------------------------
#
# Project created by QtCreator 2013-07-30T21:02:55
#
#-------------------------------------------------

include(../../config.pri)
include(../common.pri)

QT       += core multimedia

TARGET = hour_signal

SOURCES += hour_signal.cpp

HEADERS += hour_signal.h
OTHER_FILES += hour_signal.json

win32:RC_FILE = hour_signal.rc

RESOURCES += \
    hour_signal.qrc

TRANSLATIONS += \
    hour_signal_ru.ts
