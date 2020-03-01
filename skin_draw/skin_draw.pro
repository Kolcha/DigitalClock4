#
#   Digital Clock: skin draw engine
#   Copyright (C) 2013-2020  Nick Korotysh <nick.korotysh@gmail.com>
#
#   This program is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

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
    iimage_cache.h \
    raster_skin.h \
    skin_draw_global.h \
    skin_drawer.h \
    text_skin.h \
    vector_skin.h \
    zoom_type.hpp

win32:RC_FILE = skin_draw.rc
unix:VERSION = 1.5.1
macx:QMAKE_LFLAGS_SONAME = -Wl,-install_name,@executable_path/../Frameworks/

unix:!macx {
    target.path = $$clock_libs_path
    INSTALLS += target
}
