#
#   Digital Clock: tray color plugin
#   Copyright (C) 2013-2019  Nick Korotysh <nick.korotysh@gmail.com>
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
# Project created by QtCreator 2013-10-20T11:43:12
#
#-------------------------------------------------

QT       += core gui widgets

include(../common.pri)

TARGET = tray_color

SOURCES += tray_color.cpp

HEADERS += tray_color.h \
    tray_color_settings.h

include(lang/lang.pri)
include(../../qm_gen.pri)

RESOURCES += tray_color.qrc

OTHER_FILES += tray_color.json

win32:RC_FILE = tray_color.rc
