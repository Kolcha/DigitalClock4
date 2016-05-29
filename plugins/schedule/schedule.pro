#-------------------------------------------------
#
# Project created by QtCreator 2013-11-12T07:34:14
#
#-------------------------------------------------

QT       += core gui widgets

include(../common.pri)

TARGET = schedule

SOURCES += schedule.cpp \
    core/daily_tasks_provider.cpp \
    core/task.cpp \
    core/tasks_invoker.cpp \
    core/tasks_storage.cpp \
    gui/schedule_dialog.cpp \
    gui/task_edit_dialog.cpp

HEADERS += schedule.h \
    core/daily_tasks_provider.h \
    core/task.h \
    core/tasks_invoker.h \
    core/tasks_storage.h \
    gui/schedule_dialog.h \
    gui/task_edit_dialog.h

FORMS += \
    gui/schedule_dialog.ui \
    gui/task_edit_dialog.ui

TRANSLATIONS += \
    schedule_ru.ts

include(../../qm_gen.pri)

RESOURCES += schedule.qrc

OTHER_FILES += schedule.json

win32:RC_FILE = schedule.rc
