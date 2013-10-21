#-------------------------------------------------
#
# Project created by QtCreator 2013-07-17T19:57:53
#
#-------------------------------------------------

QT       += core

TARGET = spectrum_clock
TEMPLATE = lib
CONFIG += plugin

SOURCES += spectrum_clock.cpp

HEADERS += spectrum_clock.h
OTHER_FILES += spectrum_clock.json

win32:RC_FILE = spectrum_clock.rc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../clock_common/release/ -lclock_common
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../clock_common/debug/ -lclock_common
else:unix: LIBS += -L$$OUT_PWD/../../clock_common/ -lclock_common

INCLUDEPATH += $$PWD/../../clock_common
DEPENDPATH += $$PWD/../../clock_common
