#
#   Digital Clock: schedule plugin
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

#-------------------------------------------------
#
# Project created by QtCreator 2013-11-12T07:34:14
#
#-------------------------------------------------

QT       += core gui widgets multimedia

include(../common.pri)

TARGET = schedule

SOURCES += schedule.cpp \
    core/daily_tasks_provider.cpp \
    core/notification.cpp \
    core/task.cpp \
    core/tasks_invoker.cpp \
    core/tasks_storage.cpp \
    gui/schedule_dialog.cpp \
    gui/task_advanced_settings_dialog.cpp \
    gui/task_edit_dialog.cpp

HEADERS += schedule.h \
    core/daily_tasks_provider.h \
    core/notification.h \
    core/task.h \
    core/tasks_invoker.h \
    core/tasks_storage.h \
    gui/schedule_dialog.h \
    gui/task_advanced_settings_dialog.h \
    gui/task_edit_dialog.h

FORMS += \
    gui/schedule_dialog.ui \
    gui/task_advanced_settings_dialog.ui \
    gui/task_edit_dialog.ui

include(lang/lang.pri)
include(../../qm_gen.pri)

RESOURCES += schedule.qrc

OTHER_FILES += schedule.json

win32:RC_FILE = schedule.rc
