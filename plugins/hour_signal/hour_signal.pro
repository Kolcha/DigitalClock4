#-------------------------------------------------
#
# Project created by QtCreator 2013-07-30T21:02:55
#
#-------------------------------------------------

QT       += core multimedia

TARGET = hour_signal
TEMPLATE = lib
CONFIG += plugin

SOURCES += hour_signal.cpp

HEADERS += hour_signal.h
OTHER_FILES += hour_signal.json

win32:RC_FILE = hour_signal.rc

RESOURCES += \
    hour_signal.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../clock_common/release/ -lclock_common
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../clock_common/debug/ -lclock_common
else:unix: LIBS += -L$$OUT_PWD/../../clock_common/ -lclock_common

INCLUDEPATH += $$PWD/../../clock_common
DEPENDPATH += $$PWD/../../clock_common
