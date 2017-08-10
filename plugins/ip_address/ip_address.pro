#
#   Digital Clock: IP address plugin
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
# Project created by QtCreator 2016-05-27T13:13:13
#
#-------------------------------------------------

QT       += core gui widgets network

include(../common.pri)

TARGET = ip_address

SOURCES += \
    ip_address_plugin.cpp \
    core/ip_addres_settings.cpp \
    core/net_interfaces_model.cpp \
    gui/settings_dialog.cpp

HEADERS += \
    ip_address_plugin.h \
    core/ip_addres_settings.h \
    core/net_interfaces_model.h \
    gui/settings_dialog.h

FORMS += \
    gui/settings_dialog.ui

TRANSLATIONS += \
    ip_address_en.ts \
    ip_address_ru.ts

include(../../qm_gen.pri)

RESOURCES += ip_address.qrc

OTHER_FILES += ip_address.json

win32:RC_FILE = ip_address.rc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../plugin_core/release/ -lplugin_core
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../plugin_core/debug/ -lplugin_core
else:unix: LIBS += -L$$OUT_PWD/../../plugin_core/ -lplugin_core

INCLUDEPATH += $$PWD/../../plugin_core
DEPENDPATH += $$PWD/../../plugin_core
