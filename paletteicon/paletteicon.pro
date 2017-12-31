#
#   custom icon engine Qt plugin
#   Copyright (C) 2017-2018  Nick Korotysh <nick.korotysh@gmail.com>
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
# Project created by QtCreator 2017-07-26T15:16:12
#
#-------------------------------------------------

QT       += core gui svg

TARGET = paletteicon
TEMPLATE = lib
CONFIG += plugin

QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    palette_icon_engine_plugin.cpp \
    palette_icon_engine.cpp

HEADERS += \
    palette_icon_engine_plugin.h \
    palette_icon_engine.h

DISTFILES += paletteicon.json

win32:RC_FILE = paletteicon.rc
