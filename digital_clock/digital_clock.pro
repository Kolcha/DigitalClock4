#-------------------------------------------------
#
# Project created by QtCreator 2013-06-24T11:39:45
#
#-------------------------------------------------

QT       += core gui widgets network svg

include(../config.pri)

TARGET   = digital_clock
TEMPLATE = app

SOURCES += main.cpp\
    core/skin_manager.cpp \
    core/clock_settings.cpp \
    gui/settings_dialog.cpp \
    gui/about_dialog.cpp \
    core/plugin_manager.cpp \
    skin/clock_vector_skin.cpp \
    skin/clock_raster_skin.cpp \
    skin/clock_text_skin.cpp \
    skin/clock_base_skin.cpp \
    gui/plugin_info_dialog.cpp \
    core/updater.cpp \
    core/http_client.cpp \
    gui/clock_widget.cpp \
    gui/clock_display.cpp \
    gui/colorize_effect.cpp \
    gui/tray_control.cpp \
    gui/plugin_list_item_widget.cpp \
    core/manager_base.cpp \
    core/autostart.cpp \
    main_window.cpp

HEADERS += \
    core/skin_manager.h \
    core/clock_settings.h \
    gui/settings_dialog.h \
    gui/about_dialog.h \
    core/plugin_manager.h \
    skin/clock_vector_skin.h \
    skin/clock_raster_skin.h \
    skin/clock_text_skin.h \
    skin/clock_base_skin.h \
    core/plugin_info.h \
    gui/plugin_info_dialog.h \
    core/updater.h \
    core/http_client.h \
    gui/clock_widget.h \
    gui/clock_display.h \
    gui/colorize_effect.h \
    gui/tray_control.h \
    gui/plugin_list_item_widget.h \
    core/manager_base.h \
    core/autostart.h \
    core/build_defs.h \
    main_window.h

FORMS += \
    gui/settings_dialog.ui \
    gui/about_dialog.ui \
    gui/plugin_info_dialog.ui \
    gui/plugin_list_item_widget.ui

TRANSLATIONS += \
    languages/digital_clock_ru.ts

include(../qm_gen.pri)

RESOURCES += \
    digital_clock.qrc

win32:RC_FILE = digital_clock.rc
macx {
    ICON = resources/1400320218_103169.icns
    QMAKE_INFO_PLIST = resources/Info.plist
}

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
