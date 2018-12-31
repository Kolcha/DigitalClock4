#
#   Digital Clock: timetracker plugin
#   Copyright (C) 2018-2019  Nick Korotysh <nick.korotysh@gmail.com>
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
# Project created by QtCreator 2018-04-27T01:26:34
#
#-------------------------------------------------

QT       += core gui widgets

include(../common.pri)

TARGET = timetracker

SOURCES += \
    timetracker_plugin.cpp \
    timetracker.cpp \
    tracker_widget.cpp

HEADERS += \
    timetracker_plugin.h \
    timetracker.h \
    tracker_widget.h

DISTFILES += timetracker.json

win32:RC_FILE = timetracker.rc
