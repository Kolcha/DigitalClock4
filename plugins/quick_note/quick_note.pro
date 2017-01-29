#
#   Digital Clock: quick note plugin
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
# Project created by QtCreator 2013-12-29T23:04:10
#
#-------------------------------------------------

QT       += core gui widgets

include(../common.pri)

TARGET = quick_note

SOURCES += \
    quick_note_plugin.cpp \
    gui/message_widget.cpp \
    gui/settings_dialog.cpp

HEADERS += \
    quick_note_plugin.h \
    quick_note_settings.h \
    gui/message_widget.h \
    gui/settings_dialog.h

FORMS += \
    gui/message_widget.ui \
    gui/settings_dialog.ui

TRANSLATIONS += \
    quick_note_ru.ts

include(../../qm_gen.pri)

RESOURCES += quick_note.qrc

OTHER_FILES += quick_note.json

win32:RC_FILE = quick_note.rc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../plugin_core/release/ -lplugin_core
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../plugin_core/debug/ -lplugin_core
else:unix: LIBS += -L$$OUT_PWD/../../plugin_core/ -lplugin_core

INCLUDEPATH += $$PWD/../../plugin_core
DEPENDPATH += $$PWD/../../plugin_core
