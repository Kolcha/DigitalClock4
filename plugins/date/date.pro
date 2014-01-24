#-------------------------------------------------
#
# Project created by QtCreator 2014-01-01T09:46:16
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = date
TEMPLATE = lib
CONFIG += plugin c++11

SOURCES += date.cpp

HEADERS += date.h \
    date_settings.h
OTHER_FILES += date.json

RESOURCES += \
    date.qrc

TRANSLATIONS += \
    date_ru.ts

win32:RC_FILE = date.rc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../clock_common/release/ -lclock_common
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../clock_common/debug/ -lclock_common
else:unix: LIBS += -L$$OUT_PWD/../../clock_common/ -lclock_common

INCLUDEPATH += $$PWD/../../clock_common
DEPENDPATH += $$PWD/../../clock_common

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../skin_draw/release/ -lskin_draw
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../skin_draw/debug/ -lskin_draw
else:unix: LIBS += -L$$OUT_PWD/../../skin_draw/ -lskin_draw

INCLUDEPATH += $$PWD/../../skin_draw
DEPENDPATH += $$PWD/../../skin_draw
