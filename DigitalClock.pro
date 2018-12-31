#
#   Digital Clock - beautiful customizable clock with plugins
#   Copyright (C) 2013-2019  Nick Korotysh <nick.korotysh@gmail.com>
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

TEMPLATE = subdirs

SUBDIRS += \
    paletteicon \
    clock_common \
    skin_draw \
    plugin_core \
    digital_clock \
    plugins

digital_clock.depends = clock_common skin_draw plugin_core
plugin_core.depends = clock_common skin_draw
plugins.depends = plugin_core

OTHER_FILES += \
    3rdparty/qhotkey.pri \
    3rdparty/single_application.pri \
    config.pri \
    qm_gen.pri
