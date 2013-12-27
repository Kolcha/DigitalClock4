#-------------------------------------------------
#
# Project created by QtCreator 2013-10-20T11:43:12
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = tray_color
TEMPLATE = lib
CONFIG += plugin

SOURCES += tray_color.cpp

HEADERS += tray_color.h \
    tray_color_settings.h
OTHER_FILES += tray_color.json

TRANSLATIONS += \
    tray_color_ru.ts

win32:RC_FILE = tray_color.rc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../clock_common/release/ -lclock_common
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../clock_common/debug/ -lclock_common
else:unix: LIBS += -L$$OUT_PWD/../../clock_common/ -lclock_common

INCLUDEPATH += $$PWD/../../clock_common
DEPENDPATH += $$PWD/../../clock_common

RESOURCES += \
    tray_color.qrc
