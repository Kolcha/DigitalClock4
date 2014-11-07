#-------------------------------------------------
#
# Project created by QtCreator 2013-07-30T21:02:55
#
#-------------------------------------------------

QT       += core multimedia

include(../common.pri)

TARGET = hour_signal

SOURCES += hour_signal.cpp

HEADERS += hour_signal.h

TRANSLATIONS += \
    hour_signal_ru.ts

include(../../qm_gen.pri)

RESOURCES += hour_signal.qrc

OTHER_FILES += hour_signal.json

win32:RC_FILE = hour_signal.rc
