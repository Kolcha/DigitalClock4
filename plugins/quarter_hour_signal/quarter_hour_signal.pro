#
#   Digital Clock: quarter hour signal plugin
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
# Project created by QtCreator 2016-05-28T08:27:36
#
#-------------------------------------------------

QT       += core multimedia

include(../common.pri)

TARGET = quarter_hour_signal

SOURCES += quarter_hour_signal.cpp

HEADERS += quarter_hour_signal.h

TRANSLATIONS += \
    quarter_hour_signal_ru.ts

include(../../qm_gen.pri)

RESOURCES += quarter_hour_signal.qrc

OTHER_FILES += quarter_hour_signal.json

win32:RC_FILE = quarter_hour_signal.rc
