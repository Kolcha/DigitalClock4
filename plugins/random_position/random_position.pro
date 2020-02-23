#
#   Digital Clock: random position plugin
#   Copyright (C) 2020  Nick Korotysh <nick.korotysh@gmail.com>
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
# Project created by QtCreator 2020-02-23T18:59:00
#
#-------------------------------------------------

QT       += widgets

include(../common.pri)

TARGET = random_position

SOURCES += random_position.cpp

HEADERS += random_position.h

include(lang/lang.pri)
include(../../qm_gen.pri)

OTHER_FILES += random_position.json

win32:RC_FILE = random_position.rc
