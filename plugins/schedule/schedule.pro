#-------------------------------------------------
#
# Project created by QtCreator 2013-11-12T07:34:14
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = schedule
TEMPLATE = lib
CONFIG += plugin

SOURCES += schedule.cpp \
    gui/settings_dialog.cpp \
    gui/add_task_dialog.cpp \
    core/task_manager.cpp

HEADERS += schedule.h \
    gui/settings_dialog.h \
    gui/add_task_dialog.h \
    core/task.h \
    core/task_manager.h
OTHER_FILES += schedule.json

win32:RC_FILE = schedule.rc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../clock_common/release/ -lclock_common
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../clock_common/debug/ -lclock_common
else:unix: LIBS += -L$$OUT_PWD/../../clock_common/ -lclock_common

INCLUDEPATH += $$PWD/../../clock_common
DEPENDPATH += $$PWD/../../clock_common

RESOURCES += \
    schedule.qrc

FORMS += \
    gui/settings_dialog.ui \
    gui/add_task_dialog.ui
