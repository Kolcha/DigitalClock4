#
#   Digital Clock: chime plugin
#   Copyright (C) 2016-2017  Nick Korotysh <nick.korotysh@gmail.com>
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
# Project created by QtCreator 2016-10-21T22:34:33
#
#-------------------------------------------------

QT       += core gui widgets multimedia

include(../common.pri)

TARGET = chime

SOURCES += \
    chime_plugin.cpp \
    gui/settings_dialog.cpp \
    gui/sound_dialog.cpp \
    core/chime_settings.cpp

HEADERS += \
    chime_plugin.h \
    gui/settings_dialog.h \
    gui/sound_dialog.h \
    core/chime_settings.h \
    core/repeat_type.h

FORMS += \
    gui/settings_dialog.ui \
    gui/sound_dialog.ui

TRANSLATIONS += \
    lang/chime_en.ts \
    lang/chime_ru.ts

include(../../qm_gen.pri)

DISTFILES += chime.json

RESOURCES += chime.qrc

win32:RC_FILE = chime.rc
