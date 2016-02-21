#-------------------------------------------------
#
# Project created by QtCreator 2013-07-19T00:06:16
#
#-------------------------------------------------

QT       += core

include(../common.pri)

TARGET = var_translucency

SOURCES += var_translucency.cpp

HEADERS += var_translucency.h

TRANSLATIONS += \
    var_translucency_ru.ts

include(../../qm_gen.pri)

RESOURCES += var_translucency.qrc

OTHER_FILES += var_translucency.json

win32:RC_FILE = var_translucency.rc
