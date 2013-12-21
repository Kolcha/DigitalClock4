#-------------------------------------------------
#
# Project created by QtCreator 2013-10-29T23:52:02
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = any_zoom
TEMPLATE = lib
CONFIG += plugin

SOURCES += \
    any_zoom.cpp

HEADERS += \
    any_zoom.h \
    any_zoom_settings.h
OTHER_FILES += any_zoom.json

win32:RC_FILE = any_zoom.rc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../clock_common/release/ -lclock_common
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../clock_common/debug/ -lclock_common
else:unix: LIBS += -L$$OUT_PWD/../../clock_common/ -lclock_common

INCLUDEPATH += $$PWD/../../clock_common
DEPENDPATH += $$PWD/../../clock_common

RESOURCES += \
    any_zoom.qrc

TRANSLATIONS += \
    any_zoom_ru.ts
