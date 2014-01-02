#-------------------------------------------------
#
# Project created by QtCreator 2014-01-02T22:55:42
#
#-------------------------------------------------

QT       += core

TARGET = win_on_top
TEMPLATE = lib
CONFIG += plugin

SOURCES += win_on_top.cpp

HEADERS += win_on_top.h
OTHER_FILES += win_on_top.json

TRANSLATIONS += \
    win_on_top_ru.ts

RESOURCES += \
    win_on_top.qrc

win32:RC_FILE = win_on_top.rc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../clock_common/release/ -lclock_common
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../clock_common/debug/ -lclock_common
else:unix: LIBS += -L$$OUT_PWD/../../clock_common/ -lclock_common

INCLUDEPATH += $$PWD/../../clock_common
DEPENDPATH += $$PWD/../../clock_common
