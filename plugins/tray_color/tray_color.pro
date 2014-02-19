#-------------------------------------------------
#
# Project created by QtCreator 2013-10-20T11:43:12
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = tray_color

SOURCES += tray_color.cpp

HEADERS += tray_color.h \
    tray_color_settings.h
OTHER_FILES += tray_color.json

TRANSLATIONS += \
    tray_color_ru.ts

win32:RC_FILE = tray_color.rc

RESOURCES += \
    tray_color.qrc

include(../common.pri)
