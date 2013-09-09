#-------------------------------------------------
#
# Project created by QtCreator 2013-09-09T06:55:05
#
#-------------------------------------------------

QT       += core gui widgets
INCLUDEPATH += ../../core

TARGET = am_pm
TEMPLATE = lib
CONFIG += plugin

SOURCES += am_pm.cpp

HEADERS += am_pm.h \
    ../../core/iclock_plugin.h
OTHER_FILES += am_pm.json

win32:RC_FILE = am_pm.rc
