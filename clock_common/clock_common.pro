#
#   Digital Clock: clock common library
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
# Project created by QtCreator 2013-10-21T22:29:49
#
#-------------------------------------------------

QT       += core gui widgets

include(../config.pri)

TARGET   = clock_common
TEMPLATE = lib

DEFINES += CLOCK_COMMON_LIBRARY

SOURCES += \
    config_base.cpp \
    config_serialization.cpp \
    led_tool_button.cpp \
    message_box.cpp \
    settings_storage.cpp \
    settings_storage_wrapper.cpp

HEADERS += \
    clock_common_global.h \
    config_base.h \
    config_serialization.h \
    led_tool_button.h \
    message_box.h \
    settings_keys.h \
    settings_storage.h \
    settings_storage_wrapper.h

win32:RC_FILE = clock_common.rc
unix:VERSION = 1.2.0
macx:QMAKE_LFLAGS_SONAME = -Wl,-install_name,@executable_path/../Frameworks/
