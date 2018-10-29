#
#   Digital Clock: spectrum clock plugin
#   Copyright (C) 2013-2018  Nick Korotysh <nick.korotysh@gmail.com>
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
# Project created by QtCreator 2013-07-17T19:57:53
#
#-------------------------------------------------

QT       += core

include(../common.pri)

TARGET = spectrum_clock

SOURCES += spectrum_clock.cpp

HEADERS += spectrum_clock.h

include(lang/lang.pri)
include(../../qm_gen.pri)

RESOURCES += spectrum_clock.qrc

OTHER_FILES += spectrum_clock.json

win32:RC_FILE = spectrum_clock.rc
