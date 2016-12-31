#
#   Digital Clock: any zoom plugin
#   Copyright (C) 2013-2017  Nick Korotysh <nick.korotysh@gmail.com>
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
# Project created by QtCreator 2013-10-29T23:52:02
#
#-------------------------------------------------

QT       += core gui widgets

include(../common.pri)

TARGET = any_zoom

SOURCES += any_zoom.cpp

HEADERS += any_zoom.h \
    any_zoom_settings.h

TRANSLATIONS += \
    any_zoom_ru.ts

include(../../qm_gen.pri)

RESOURCES += any_zoom.qrc

OTHER_FILES += any_zoom.json

win32:RC_FILE = any_zoom.rc
