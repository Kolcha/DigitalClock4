#
#   Digital Clock: plugin core library
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
# Project created by QtCreator 2016-09-04T09:33:27
#
#-------------------------------------------------

QT       += core gui widgets

include(../config.pri)

TARGET = plugin_core
TEMPLATE = lib

DEFINES += PLUGIN_CORE_LIBRARY

SOURCES += \
    base_settings_widget.cpp \
    widget_plugin_base.cpp \
    widget_plugin_base_private.cpp \
    widget_plugin_settings.cpp

HEADERS += \
    base_settings_widget.h \
    iskin_user_plugin.h \
    plugin_core_global.h \
    widget_plugin_base.h \
    widget_plugin_base_private.h \
    widget_plugin_settings.h

FORMS += \
    base_settings_widget.ui

TRANSLATIONS += \
    plugin_core_en.ts \
    plugin_core_ru.ts

include(../qm_gen.pri)

RESOURCES += \
    plugin_core.qrc

win32:RC_FILE = plugin_core.rc
unix:VERSION = 1.1.3
macx:QMAKE_LFLAGS_SONAME = -Wl,-install_name,@executable_path/../Frameworks/

# add skin_draw library
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../skin_draw/release/ -lskin_draw
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../skin_draw/debug/ -lskin_draw
else:unix: LIBS += -L$$OUT_PWD/../skin_draw/ -lskin_draw

INCLUDEPATH += $$PWD/../skin_draw
DEPENDPATH += $$PWD/../skin_draw

# add clock_common library
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../clock_common/release/ -lclock_common
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../clock_common/debug/ -lclock_common
else:unix: LIBS += -L$$OUT_PWD/../clock_common/ -lclock_common

INCLUDEPATH += $$PWD/../clock_common
DEPENDPATH += $$PWD/../clock_common
