#
#   Digital Clock - beautiful customizable clock with plugins
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
# Project created by QtCreator 2013-06-24T11:39:45
#
#-------------------------------------------------

QT       += core gui widgets network svg
unix:!macx {
    QT += x11extras
}

include(../config.pri)

TARGET   = digital_clock
TEMPLATE = app

SOURCES += main.cpp \
    core/build_date.cpp \
    core/clock_application.cpp \
    core/clock_settings.cpp \
    core/clock_state.cpp \
    core/http_client.cpp \
    core/plugin_manager.cpp \
    core/skin_manager.cpp \
    core/updater.cpp \
    gui/about_dialog.cpp \
    gui/clickable_label.cpp \
    gui/clock_display.cpp \
    gui/clock_widget.cpp \
    gui/clock_window.cpp \
    gui/colorize_effect.cpp \
    gui/context_menu.cpp \
    gui/plugin_info_dialog.cpp \
    gui/plugin_list_item_widget.cpp \
    gui/settings_dialog.cpp \
    gui/tray_control.cpp \
    skin/clock_base_skin.cpp \
    skin/clock_raster_skin.cpp \
    skin/clock_text_skin.cpp \
    skin/clock_vector_skin.cpp

HEADERS += \
    platform/autostart.h \
    core/build_date.h \
    core/build_defs.h \
    core/clock_application.h \
    core/clock_settings.h \
    core/clock_state.h \
    core/http_client.h \
    core/plugin_info.h \
    core/plugin_manager.h \
    core/skin_manager.h \
    core/updater.h \
    gui/about_dialog.h \
    gui/clickable_label.h \
    gui/clock_display.h \
    gui/clock_widget.h \
    gui/clock_window.h \
    gui/colorize_effect.h \
    gui/context_menu.h \
    gui/plugin_info_dialog.h \
    gui/plugin_list_item_widget.h \
    gui/settings_dialog.h \
    gui/tray_control.h \
    skin/clock_base_skin.h \
    skin/clock_raster_skin.h \
    skin/clock_text_skin.h \
    skin/clock_vector_skin.h

# platform-specific sources
win32 {
SOURCES += \
    platform/autostart_win.cpp \
    platform/fullscreen_detect.cpp \
    platform/main_window_win.cpp

HEADERS += \
    platform/fullscreen_detect.h
}

unix {
macx {
SOURCES += \
    platform/autostart_mac.cpp \
    platform/main_window_mac.cpp
} else {
SOURCES += \
    platform/autostart_linux.cpp \
    platform/main_window_linux.cpp
}
}

FORMS += \
    gui/about_dialog.ui \
    gui/plugin_info_dialog.ui \
    gui/plugin_list_item_widget.ui \
    gui/settings_dialog.ui

TRANSLATIONS += \
    languages/digital_clock_en.ts \
    languages/digital_clock_ru.ts

include(../qm_gen.pri)

RESOURCES += \
    resources/icons.qrc \
    resources/skins.qrc

win32:RC_FILE = digital_clock.rc
macx {
    ICON = resources/clock_icon_mac.icns
    QMAKE_INFO_PLIST = resources/Info.plist
    LIBS += -lobjc
}
unix:!macx {
    LIBS += -lX11
}
win32:LIBS += -luser32

# add plugin_core library
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../plugin_core/release/ -lplugin_core
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../plugin_core/debug/ -lplugin_core
else:unix: LIBS += -L$$OUT_PWD/../plugin_core/ -lplugin_core

INCLUDEPATH += $$PWD/../plugin_core
DEPENDPATH += $$PWD/../plugin_core

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
