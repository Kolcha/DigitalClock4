#
#   Digital Clock: date plugin
#   Copyright (C) 2014-2018  Nick Korotysh <nick.korotysh@gmail.com>
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
# Project created by QtCreator 2014-01-01T09:46:16
#
#-------------------------------------------------

QT       += core gui widgets

include(../common.pri)

TARGET = date

SOURCES += \
    date_plugin.cpp \
    gui/settings_dialog.cpp

HEADERS += \
    date_plugin.h \
    date_settings.h \
    gui/settings_dialog.h

FORMS += \
    gui/settings_dialog.ui

TRANSLATIONS += \
    lang/date_en.ts \
    lang/date_ru.ts

include(../../qm_gen.pri)

RESOURCES += date.qrc

OTHER_FILES += date.json

win32:RC_FILE = date.rc
