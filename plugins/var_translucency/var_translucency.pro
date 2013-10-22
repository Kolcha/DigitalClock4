#-------------------------------------------------
#
# Project created by QtCreator 2013-07-19T00:06:16
#
#-------------------------------------------------

QT       += core

TARGET = var_translucency
TEMPLATE = lib
CONFIG += plugin

SOURCES += var_translucency.cpp

HEADERS += var_translucency.h
OTHER_FILES += var_translucency.json

win32:RC_FILE = var_translucency.rc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../clock_common/release/ -lclock_common
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../clock_common/debug/ -lclock_common
else:unix: LIBS += -L$$OUT_PWD/../../clock_common/ -lclock_common

INCLUDEPATH += $$PWD/../../clock_common
DEPENDPATH += $$PWD/../../clock_common
