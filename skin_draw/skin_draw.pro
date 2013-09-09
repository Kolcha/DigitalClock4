#-------------------------------------------------
#
# Project created by QtCreator 2013-09-09T21:13:15
#
#-------------------------------------------------

QT       += core gui svg

TARGET = skin_draw
TEMPLATE = lib

DEFINES += SKIN_DRAW_LIBRARY

SOURCES += skin_drawer.cpp \
    vector_skin.cpp \
    skin_factory.cpp \
    raster_skin.cpp \
    base_skin.cpp \
    text_skin.cpp

HEADERS += skin_drawer.h\
        skin_draw_global.h \
    vector_skin.h \
    raster_skin.h \
    iclock_skin.h \
    base_skin.h \
    text_skin.h

win32:RC_FILE = skin_draw.rc
