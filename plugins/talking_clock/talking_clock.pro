#
#   Digital Clock: talking clock plugin
#   Copyright (C) 2016-2018  Nick Korotysh <nick.korotysh@gmail.com>
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
# Project created by QtCreator 2016-11-13T23:09:54
#
#-------------------------------------------------

QT       += core gui widgets texttospeech

include(../common.pri)

TARGET = talking_clock

SOURCES += \
    talking_clock_plugin.cpp \
    gui/settings_dialog.cpp \
    talking_clock_settings.cpp \
    gui/voice_config_dialog.cpp

HEADERS += \
    talking_clock_plugin.h \
    gui/settings_dialog.h \
    talking_clock_settings.h \
    gui/voice_config_dialog.h

FORMS += \
    gui/settings_dialog.ui \
    gui/voice_config_dialog.ui

include(lang/lang.pri)
include(../../qm_gen.pri)

DISTFILES += talking_clock.json

RESOURCES += talking_clock.qrc

win32:RC_FILE = talking_clock.rc
