#
#   Digital Clock: tower clock signal plugin
#   Copyright (C) 2016  Nick Korotysh <nick.korotysh@gmail.com>
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
# Project created by QtCreator 2016-03-22T09:03:32
#
#-------------------------------------------------

QT       += core multimedia

include(../common.pri)

TARGET = tower_clock_signal

SOURCES += tower_clock_signal.cpp

HEADERS += tower_clock_signal.h

DISTFILES += tower_clock_signal.json

TRANSLATIONS += \
    tower_clock_signal_ru.ts

include(../../qm_gen.pri)

RESOURCES += tower_clock_signal.qrc

win32:RC_FILE = tower_clock_signal.rc
