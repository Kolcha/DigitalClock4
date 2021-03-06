#
#   Digital Clock - beautiful customizable clock with plugins
#   Copyright (C) 2014-2020  Nick Korotysh <nick.korotysh@gmail.com>
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

CONFIG += strict_c++ c++14

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_USE_QSTRINGBUILDER

win32-msvc*: QMAKE_LFLAGS_RELEASE += /RELEASE

unix:!macx {
    isEmpty(prefix): prefix = /usr/local
    isEmpty(lib_dir): lib_dir = $$prefix/lib
    clock_libs_path = $$lib_dir/digitalclock4
}
