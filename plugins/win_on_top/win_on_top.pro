#-------------------------------------------------
#
# Project created by QtCreator 2014-01-02T22:55:42
#
#-------------------------------------------------

QT       += core

include(../common.pri)

TARGET = win_on_top

SOURCES += win_on_top.cpp

HEADERS += win_on_top.h

TRANSLATIONS += \
    win_on_top_ru.ts

include(../../qm_gen.pri)

RESOURCES += win_on_top.qrc

OTHER_FILES += win_on_top.json

win32:RC_FILE = win_on_top.rc
