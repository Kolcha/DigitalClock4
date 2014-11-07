#-------------------------------------------------
#
# Project created by QtCreator 2013-10-20T11:43:12
#
#-------------------------------------------------

QT       += core gui widgets

include(../common.pri)

TARGET = tray_color

SOURCES += tray_color.cpp

HEADERS += tray_color.h \
    tray_color_settings.h

TRANSLATIONS += \
    tray_color_ru.ts

include(../../qm_gen.pri)

RESOURCES += tray_color.qrc

OTHER_FILES += tray_color.json

win32:RC_FILE = tray_color.rc
