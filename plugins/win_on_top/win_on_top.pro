#-------------------------------------------------
#
# Project created by QtCreator 2014-01-02T22:55:42
#
#-------------------------------------------------

QT       += core

TARGET = win_on_top

SOURCES += win_on_top.cpp

HEADERS += win_on_top.h
OTHER_FILES += win_on_top.json

TRANSLATIONS += \
    win_on_top_ru.ts

RESOURCES += \
    win_on_top.qrc

win32:RC_FILE = win_on_top.rc

include(../common.pri)
