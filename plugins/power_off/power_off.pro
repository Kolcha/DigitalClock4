#-------------------------------------------------
#
# Project created by QtCreator 2013-12-26T22:00:43
#
#-------------------------------------------------

include(../../config.pri)

QT       += core gui widgets

TARGET = power_off
TEMPLATE = lib
CONFIG += plugin

SOURCES += power_off.cpp \
    gui/settings_dialog.cpp

HEADERS += power_off.h \
    gui/settings_dialog.h \
    power_off_settings.h
OTHER_FILES += power_off.json

win32:RC_FILE = power_off.rc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../clock_common/release/ -lclock_common
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../clock_common/debug/ -lclock_common
else:unix: LIBS += -L$$OUT_PWD/../../clock_common/ -lclock_common

INCLUDEPATH += $$PWD/../../clock_common
DEPENDPATH += $$PWD/../../clock_common

RESOURCES += \
    power_off.qrc

TRANSLATIONS += \
    power_off_ru.ts

FORMS += \
    gui/settings_dialog.ui

win32:LIBS += -lAdvapi32
