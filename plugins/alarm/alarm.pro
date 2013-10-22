#-------------------------------------------------
#
# Project created by QtCreator 2013-08-13T20:52:25
#
#-------------------------------------------------

QT       += core gui widgets multimedia

TARGET = alarm
TEMPLATE = lib
CONFIG += plugin

SOURCES += alarm.cpp \
    gui/settings_dlg.cpp

HEADERS += alarm.h \
    gui/settings_dlg.h \
    alarm_settings.h
OTHER_FILES += alarm.json

win32:RC_FILE = alarm.rc

FORMS += \
    gui/settings_dlg.ui

RESOURCES += \
    alarm.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../clock_common/release/ -lclock_common
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../clock_common/debug/ -lclock_common
else:unix: LIBS += -L$$OUT_PWD/../../clock_common/ -lclock_common

INCLUDEPATH += $$PWD/../../clock_common
DEPENDPATH += $$PWD/../../clock_common
