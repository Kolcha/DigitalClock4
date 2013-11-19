#-------------------------------------------------
#
# Project created by QtCreator 2013-11-12T07:34:14
#
#-------------------------------------------------

QT       += core gui widgets
CONFIG += c++11

TARGET = schedule
TEMPLATE = lib
CONFIG += plugin

SOURCES += schedule.cpp \
    core/task_manager.cpp \
    gui/add_task_dlg.cpp \
    gui/settings_dlg.cpp

HEADERS += schedule.h \
    core/task.h \
    core/task_manager.h \
    core/schedule_settings.h \
    gui/add_task_dlg.h \
    gui/settings_dlg.h
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
    gui/add_task_dlg.ui \
    gui/settings_dlg.ui
