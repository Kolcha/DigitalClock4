#
#   Digital Clock: alarm plugin
#   Copyright (C) 2013-2017  Nick Korotysh <nick.korotysh@gmail.com>
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
# Project created by QtCreator 2013-08-13T20:52:25
#
#-------------------------------------------------

QT       += core gui widgets multimedia

include(../common.pri)

TARGET = alarm

SOURCES += alarm.cpp \
    core/alarm_item.cpp \
    core/alarms_storage.cpp \
    gui/alarm_list_item_widget.cpp \
    gui/edit_alarm_dialog.cpp \
    gui/settings_dialog.cpp

HEADERS += alarm.h \
    alarm_settings.h \
    core/alarm_item.h \
    core/alarms_storage.h \
    gui/alarm_list_item_widget.h \
    gui/edit_alarm_dialog.h \
    gui/settings_dialog.h

FORMS += \
    gui/alarm_list_item_widget.ui \
    gui/edit_alarm_dialog.ui \
    gui/settings_dialog.ui

TRANSLATIONS += \
    alarm_ru.ts

include(../../qm_gen.pri)

RESOURCES += alarm.qrc

OTHER_FILES += alarm.json

win32:RC_FILE = alarm.rc
