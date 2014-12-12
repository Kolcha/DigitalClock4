#-------------------------------------------------
#
# Project created by QtCreator 2013-12-26T22:00:43
#
#-------------------------------------------------

QT       += core gui widgets

include(../common.pri)

TARGET = power_off

SOURCES += power_off.cpp \
    gui/settings_dialog.cpp

HEADERS += power_off.h \
    gui/settings_dialog.h \
    power_off_settings.h

FORMS += \
    gui/settings_dialog.ui

TRANSLATIONS += \
    power_off_ru.ts

include(../../qm_gen.pri)

RESOURCES += power_off.qrc

OTHER_FILES += power_off.json

win32:RC_FILE = power_off.rc
win32:LIBS += -lAdvapi32 -luser32
