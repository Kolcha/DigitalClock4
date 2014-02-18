#-------------------------------------------------
#
# Project created by QtCreator 2013-12-29T23:04:10
#
#-------------------------------------------------

include(../../config.pri)

QT       += core gui widgets

TARGET = quick_note
TEMPLATE = lib
CONFIG += plugin

SOURCES += quick_note.cpp

HEADERS += quick_note.h \
    quick_note_settings.h
OTHER_FILES += quick_note.json

RESOURCES += \
    quick_note.qrc

TRANSLATIONS += \
    quick_note_ru.ts

win32:RC_FILE = quick_note.rc

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
