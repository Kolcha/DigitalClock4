#
#   window on top hook library
#   Copyright (C) 2017-2018  Nick Korotysh <nick.korotysh@gmail.com>
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
# Project created by QtCreator 2017-08-05T09:08:10
#
#-------------------------------------------------

CONFIG -= qt

TARGET = win_on_top_hook
TEMPLATE = lib

DEFINES += _WINDOWS _USRDLL

SOURCES += win_on_top_hook.cpp

RC_FILE = win_on_top_hook.rc

LIBS += -luser32
