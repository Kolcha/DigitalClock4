#-------------------------------------------------
#
# Project created by QtCreator 2013-06-24T11:39:45
#
#-------------------------------------------------

QT       += core gui svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DigitalClock4
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    skin/base_skin.cpp \
    skin/raster_skin.cpp \
    skin/vector_skin.cpp \
    skin/skin_factory.cpp

HEADERS  += widget.h \
    skin/iclock_skin.h \
    skin/base_skin.h \
    skin/raster_skin.h \
    skin/vector_skin.h

FORMS    += widget.ui
