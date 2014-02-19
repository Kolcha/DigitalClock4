#-------------------------------------------------
#
# Project created by QtCreator 2014-01-01T09:46:16
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = date

SOURCES += date.cpp \
    gui/settings_dialog.cpp

HEADERS += date.h \
    date_settings.h \
    gui/settings_dialog.h
OTHER_FILES += date.json

RESOURCES += \
    date.qrc

TRANSLATIONS += \
    date_ru.ts

win32:RC_FILE = date.rc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../skin_draw/release/ -lskin_draw
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../skin_draw/debug/ -lskin_draw
else:unix: LIBS += -L$$OUT_PWD/../../skin_draw/ -lskin_draw

INCLUDEPATH += $$PWD/../../skin_draw
DEPENDPATH += $$PWD/../../skin_draw

FORMS += \
    gui/settings_dialog.ui

include(../common.pri)
