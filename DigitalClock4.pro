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
    skin/skin_factory.cpp \
    skin/skin_drawer.cpp \
    core/skin_manager.cpp \
    core/clock_settings.cpp \
    gui/digital_clock.cpp

HEADERS  += widget.h \
    skin/iclock_skin.h \
    skin/base_skin.h \
    skin/raster_skin.h \
    skin/vector_skin.h \
    skin/skin_drawer.h \
    core/skin_manager.h \
    core/clock_settings.h \
    gui/digital_clock.h

FORMS    += widget.ui
