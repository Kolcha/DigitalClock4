#-------------------------------------------------
#
# Project created by QtCreator 2013-10-29T23:52:02
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = extra_zoom
TEMPLATE = lib
CONFIG += plugin

SOURCES += \
    extra_zoom.cpp

HEADERS += \
    extra_zoom.h \
    extra_zoom_settings.h
OTHER_FILES += extra_zoom.json

win32:RC_FILE = extra_zoom.rc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../clock_common/release/ -lclock_common
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../clock_common/debug/ -lclock_common
else:unix: LIBS += -L$$OUT_PWD/../../clock_common/ -lclock_common

INCLUDEPATH += $$PWD/../../clock_common
DEPENDPATH += $$PWD/../../clock_common
