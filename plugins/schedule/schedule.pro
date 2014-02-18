#-------------------------------------------------
#
# Project created by QtCreator 2013-11-12T07:34:14
#
#-------------------------------------------------

include(../../config.pri)
include(../common.pri)

QT       += core gui widgets

TARGET = schedule

SOURCES += schedule.cpp \
    core/task_manager.cpp \
    gui/settings_dialog.cpp \
    gui/add_task_dialog.cpp

HEADERS += schedule.h \
    core/task.h \
    core/task_manager.h \
    core/schedule_settings.h \
    gui/settings_dialog.h \
    gui/add_task_dialog.h
OTHER_FILES += schedule.json

win32:RC_FILE = schedule.rc

RESOURCES += \
    schedule.qrc

FORMS += \
    gui/settings_dialog.ui \
    gui/add_task_dialog.ui

TRANSLATIONS += \
    schedule_ru.ts
