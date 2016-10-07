#
#   Digital Clock: hour signal plugin
#   Copyright (C) 2013-2016  Nick Korotysh <nick.korotysh@gmail.com>
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
# Project created by QtCreator 2013-07-30T21:02:55
#
#-------------------------------------------------

QT       += core multimedia

include(../common.pri)

TARGET = hour_signal

SOURCES += hour_signal.cpp

HEADERS += hour_signal.h

TRANSLATIONS += \
    hour_signal_ru.ts

include(../../qm_gen.pri)

RESOURCES += hour_signal.qrc

OTHER_FILES += hour_signal.json

win32:RC_FILE = hour_signal.rc
