#-------------------------------------------------
#
# Project created by QtCreator 2016-05-28T08:27:36
#
#-------------------------------------------------

QT       += core multimedia

include(../common.pri)

TARGET = quarter_hour_signal

SOURCES += quarter_hour_signal.cpp

HEADERS += quarter_hour_signal.h

TRANSLATIONS += \
    quarter_hour_signal_ru.ts

include(../../qm_gen.pri)

RESOURCES += quarter_hour_signal.qrc

OTHER_FILES += quarter_hour_signal.json

win32:RC_FILE = quarter_hour_signal.rc
