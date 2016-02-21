#-------------------------------------------------
#
# Project created by QtCreator 2013-10-29T23:52:02
#
#-------------------------------------------------

QT       += core gui widgets

include(../common.pri)

TARGET = any_zoom

SOURCES += any_zoom.cpp

HEADERS += any_zoom.h \
    any_zoom_settings.h

TRANSLATIONS += \
    any_zoom_ru.ts

include(../../qm_gen.pri)

RESOURCES += any_zoom.qrc

OTHER_FILES += any_zoom.json

win32:RC_FILE = any_zoom.rc
