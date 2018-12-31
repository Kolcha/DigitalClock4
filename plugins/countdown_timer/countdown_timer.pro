#
#   Digital Clock: countdown timer plugin
#   Copyright (C) 2017-2019  Nick Korotysh <nick.korotysh@gmail.com>
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
# Project created by QtCreator 2017-08-30T10:37:36
#
#-------------------------------------------------

QT       += core gui widgets

include(../common.pri)

TARGET = countdown_timer

SOURCES += \
    countdown_timer_plugin.cpp \
    core/countdown_timer.cpp \
    core/settings.cpp \
    core/utilities.cpp \
    gui/settings_dialog.cpp

HEADERS += \
    countdown_timer_plugin.h \
    core/countdown_timer.h \
    core/settings.h \
    core/utilities.h \
    gui/settings_dialog.h

FORMS += \
    gui/settings_dialog.ui

include(lang/lang.pri)
include(../../qm_gen.pri)

RESOURCES += countdown_timer.qrc

OTHER_FILES += countdown_timer.json

win32:RC_FILE = countdown_timer.rc
