#
#   Digital Clock: move clock plugin
#   Copyright (C) 2014-2016  Nick Korotysh <nick.korotysh@gmail.com>
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
# Project created by QtCreator 2014-05-10T23:40:07
#
#-------------------------------------------------

QT       += core gui widgets

include(../common.pri)

TARGET = move_clock

SOURCES += move_clock.cpp \
    gui/settings_dialog.cpp

HEADERS += move_clock.h \
    gui/settings_dialog.h \
    move_clock_settings.h

FORMS += \
    gui/settings_dialog.ui

TRANSLATIONS += \
    move_clock_ru.ts

include(../../qm_gen.pri)

RESOURCES += move_clock.qrc

OTHER_FILES += move_clock.json

win32::RC_FILE = move_clock.rc
