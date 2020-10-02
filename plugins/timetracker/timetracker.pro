#
#   Digital Clock: timetracker plugin
#   Copyright (C) 2018-2020  Nick Korotysh <nick.korotysh@gmail.com>
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
include(../../3rdparty/qhotkey.pri)

TARGET = timetracker

SOURCES += \
    core/settings.cpp \
    gui/settings_dialog.cpp \
    timetracker_plugin.cpp \
    timetracker.cpp \
    tracker_widget.cpp

HEADERS += \
    core/settings.h \
    gui/settings_dialog.h \
    timetracker_plugin.h \
    timetracker.h \
    tracker_widget.h

FORMS += \
    gui/settings_dialog.ui

include(lang/lang.pri)
include(../../qm_gen.pri)

DISTFILES += timetracker.json

win32:RC_FILE = timetracker.rc
