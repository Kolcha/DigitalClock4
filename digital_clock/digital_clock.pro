#-------------------------------------------------
#
# Project created by QtCreator 2013-06-24T11:39:45
#
#-------------------------------------------------

QT       += core gui widgets
CONFIG += c++11

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
    gui/plugin_list_item.cpp \
    skin/clock_vector_skin.cpp \
    skin/clock_raster_skin.cpp \
    skin/clock_text_skin.cpp \
    skin/clock_base_skin.cpp

HEADERS  += \
    core/skin_manager.h \
    core/clock_settings.h \
    gui/digital_clock.h \
    gui/main_window.h \
    gui/tray_control.h \
    gui/settings_dialog.h \
    gui/about_dialog.h \
    core/plugin_manager.h \
    gui/plugin_list_item.h \
    skin/clock_vector_skin.h \
    skin/clock_raster_skin.h \
    skin/clock_text_skin.h \
    skin/clock_base_skin.h \
    core/plugin_info.h

FORMS    += \
    gui/settings_dialog.ui \
    gui/about_dialog.ui

RESOURCES += \
    digital_clock.qrc

TRANSLATIONS += \
    languages/digital_clock_en.ts \
    languages/digital_clock_ru.ts

win32:RC_FILE = digital_clock.rc

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
