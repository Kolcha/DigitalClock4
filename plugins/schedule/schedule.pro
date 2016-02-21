#-------------------------------------------------
#
# Project created by QtCreator 2013-11-12T07:34:14
#
#-------------------------------------------------

QT       += core gui widgets

include(../common.pri)

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

FORMS += \
    gui/settings_dialog.ui \
    gui/add_task_dialog.ui

TRANSLATIONS += \
    schedule_ru.ts

include(../../qm_gen.pri)

RESOURCES += schedule.qrc

OTHER_FILES += schedule.json

win32:RC_FILE = schedule.rc
