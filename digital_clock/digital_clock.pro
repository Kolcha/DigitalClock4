#-------------------------------------------------
#
# Project created by QtCreator 2013-06-24T11:39:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = digital_clock
TEMPLATE = app


SOURCES += main.cpp\
    core/skin_manager.cpp \
    core/clock_settings.cpp \
    gui/digital_clock.cpp \
    gui/main_window.cpp \
    gui/tray_control.cpp \
    gui/settings_dialog.cpp \
    gui/about_dialog.cpp \
    core/plugin_manager.cpp \
    gui/plugin_list_item.cpp

HEADERS  += \
    core/skin_manager.h \
    core/clock_settings.h \
    gui/digital_clock.h \
    core/settings_keys.h \
    gui/main_window.h \
    gui/tray_control.h \
    gui/settings_dialog.h \
    gui/about_dialog.h \
    core/iclock_plugin.h \
    core/plugin_manager.h \
    gui/plugin_list_item.h

FORMS    += \
    gui/settings_dialog.ui \
    gui/about_dialog.ui

RESOURCES += \
    resources/resources.qrc

TRANSLATIONS += \
    languages/digital_clock_en.ts \
    languages/digital_clock_ru.ts

win32:RC_FILE = resources/resources.rc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../skin_draw/release/ -lskin_draw
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../skin_draw/debug/ -lskin_draw
else:unix: LIBS += -L$$OUT_PWD/../skin_draw/ -lskin_draw

INCLUDEPATH += $$PWD/../skin_draw
DEPENDPATH += $$PWD/../skin_draw
