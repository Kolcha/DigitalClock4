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

QT += svg               # almost all plugins have SVG icon
TEMPLATE = lib
CONFIG += plugin

include($$PWD/../config.pri)

unix:!macx {
    target.path = $$clock_libs_path/plugins
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../plugin_core/release/ -lplugin_core
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../plugin_core/debug/ -lplugin_core
else:unix: LIBS += -L$$OUT_PWD/../../plugin_core/ -lplugin_core

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../clock_common/release/ -lclock_common
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../clock_common/debug/ -lclock_common
else:unix: LIBS += -L$$OUT_PWD/../../clock_common/ -lclock_common

INCLUDEPATH += $$PWD/../plugin_core
DEPENDPATH += $$PWD/../plugin_core

INCLUDEPATH += $$PWD/../clock_common
DEPENDPATH += $$PWD/../clock_common

INCLUDEPATH += $$PWD/../skin_draw
DEPENDPATH += $$PWD/../skin_draw

*-g++*:QMAKE_CXXFLAGS += -Wno-switch
