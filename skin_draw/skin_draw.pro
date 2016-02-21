#-------------------------------------------------
#
# Project created by QtCreator 2013-09-09T21:13:15
#
#-------------------------------------------------

QT       += core gui svg

include(../config.pri)

TARGET   = skin_draw
TEMPLATE = lib

DEFINES += SKIN_DRAW_LIBRARY

SOURCES += \
    base_skin.cpp \
    raster_skin.cpp \
    skin_drawer.cpp \
    text_skin.cpp \
    vector_skin.cpp

HEADERS += \
    base_skin.h \
    iskin.h \
    raster_skin.h \
    skin_draw_global.h \
    skin_drawer.h \
    text_skin.h \
    vector_skin.h

win32:RC_FILE = skin_draw.rc
unix:VERSION = 1.2.3
macx:QMAKE_LFLAGS_SONAME = -Wl,-install_name,@executable_path/../Frameworks/
