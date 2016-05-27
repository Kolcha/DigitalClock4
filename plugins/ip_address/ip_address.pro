#-------------------------------------------------
#
# Project created by QtCreator 2016-05-27T13:13:13
#
#-------------------------------------------------

QT       += core gui widgets network

include(../common.pri)

TARGET = ip_address

SOURCES += ip_address.cpp

HEADERS += ip_address.h

TRANSLATIONS += \
    ip_address_ru.ts

include(../../qm_gen.pri)

RESOURCES += ip_address.qrc

OTHER_FILES += ip_address.json

win32:RC_FILE = ip_address.rc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../skin_draw/release/ -lskin_draw
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../skin_draw/debug/ -lskin_draw
else:unix: LIBS += -L$$OUT_PWD/../../skin_draw/ -lskin_draw

INCLUDEPATH += $$PWD/../../skin_draw
DEPENDPATH += $$PWD/../../skin_draw
