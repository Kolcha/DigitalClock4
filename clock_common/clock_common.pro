#-------------------------------------------------
#
# Project created by QtCreator 2013-10-21T22:29:49
#
#-------------------------------------------------

QT       += core gui widgets

include(../config.pri)

TARGET   = clock_common
TEMPLATE = lib

DEFINES += CLOCK_COMMON_LIBRARY

SOURCES += \
    config_base.cpp \
    message_box.cpp \
    plugin_base.cpp \
    plugin_settings.cpp \
    settings_storage.cpp \
    settings_storage_wrapper.cpp

HEADERS += \
    clock_common_global.h \
    config_base.h \
    iclock_plugin.h \
    iplugin_init.h \
    message_box.h \
    plugin_base.h \
    plugin_settings.h \
    settings_keys.h \
    settings_storage.h \
    settings_storage_wrapper.h

win32:RC_FILE = clock_common.rc
unix:VERSION = 1.1.1
macx:QMAKE_LFLAGS_SONAME = -Wl,-install_name,@executable_path/../Frameworks/
