#-------------------------------------------------
#
# Project created by QtCreator 2013-10-29T23:52:02
#
#-------------------------------------------------

include(../../config.pri)
include(../common.pri)

QT       += core gui widgets

TARGET = any_zoom

SOURCES += \
    any_zoom.cpp

HEADERS += \
    any_zoom.h \
    any_zoom_settings.h
OTHER_FILES += any_zoom.json

win32:RC_FILE = any_zoom.rc

RESOURCES += \
    any_zoom.qrc

TRANSLATIONS += \
    any_zoom_ru.ts
