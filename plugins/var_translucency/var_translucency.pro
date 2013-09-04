#-------------------------------------------------
#
# Project created by QtCreator 2013-07-19T00:06:16
#
#-------------------------------------------------

QT       += core
INCLUDEPATH += ../../core

TARGET = var_translucency
TEMPLATE = lib
CONFIG += plugin

SOURCES += var_translucency.cpp

HEADERS += var_translucency.h \
    ../../core/iclock_plugin.h
OTHER_FILES += var_translucency.json

win32:RC_FILE = var_translucency.rc
