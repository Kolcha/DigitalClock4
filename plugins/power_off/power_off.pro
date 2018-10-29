#
#   Digital Clock: power off plugin
#   Copyright (C) 2013-2018  Nick Korotysh <nick.korotysh@gmail.com>
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
# Project created by QtCreator 2013-12-26T22:00:43
#
#-------------------------------------------------

QT       += core gui widgets

include(../common.pri)

TARGET = power_off

SOURCES += power_off.cpp \
    gui/settings_dialog.cpp

HEADERS += power_off.h \
    gui/settings_dialog.h \
    power_off_settings.h

FORMS += \
    gui/settings_dialog.ui

TRANSLATIONS += \
    lang/power_off_en.ts \
    lang/power_off_pt_PT.ts \
    lang/power_off_ru.ts

include(../../qm_gen.pri)

RESOURCES += power_off.qrc

OTHER_FILES += power_off.json

win32:RC_FILE = power_off.rc
win32:LIBS += -lAdvapi32 -luser32
