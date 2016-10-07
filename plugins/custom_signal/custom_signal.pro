#
#   Digital Clock: custom signal plugin
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
# Project created by QtCreator 2016-09-11T12:31:25
#
#-------------------------------------------------

QT       += core gui widgets multimedia

include(../common.pri)

TARGET = custom_signal

SOURCES += \
    core/signal_item.cpp \
    gui/settings_dialog.cpp \
    custom_signal_plugin.cpp \
    core/custom_signal_settings.cpp \
    gui/signal_config_dialog.cpp

HEADERS += \
    core/signal_item.h \
    gui/settings_dialog.h \
    custom_signal_plugin.h \
    core/custom_signal_settings.h \
    gui/signal_config_dialog.h

FORMS += \
    gui/settings_dialog.ui \
    gui/signal_config_dialog.ui

TRANSLATIONS += \
    custom_signal_ru.ts

include(../../qm_gen.pri)

DISTFILES += custom_signal.json

RESOURCES += \
    custom_signal.qrc

win32:RC_FILE = custom_signal.rc
