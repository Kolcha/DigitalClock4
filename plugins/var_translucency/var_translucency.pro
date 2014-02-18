#-------------------------------------------------
#
# Project created by QtCreator 2013-07-19T00:06:16
#
#-------------------------------------------------

include(../../config.pri)
include(../common.pri)

QT       += core

TARGET = var_translucency

SOURCES += var_translucency.cpp

HEADERS += var_translucency.h
OTHER_FILES += var_translucency.json

TRANSLATIONS += \
    var_translucency_ru.ts

win32:RC_FILE = var_translucency.rc

RESOURCES += \
    var_translucency.qrc
