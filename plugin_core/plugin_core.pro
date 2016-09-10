#-------------------------------------------------
#
# Project created by QtCreator 2016-09-04T09:33:27
#
#-------------------------------------------------

QT       += core gui widgets

include(../config.pri)

TARGET = plugin_core
TEMPLATE = lib

DEFINES += PLUGIN_CORE_LIBRARY

SOURCES += \
    base_settings_widget.cpp \
    widget_plugin_base.cpp \
    widget_plugin_base_private.cpp \
    widget_plugin_settings.cpp

HEADERS += \
    base_settings_widget.h \
    plugin_core_global.h \
    widget_plugin_base.h \
    widget_plugin_base_private.h \
    widget_plugin_settings.h

FORMS += \
    base_settings_widget.ui

TRANSLATIONS += \
    plugin_core_ru.ts

include(../qm_gen.pri)

RESOURCES += \
    plugin_core.qrc

win32:RC_FILE = plugin_core.rc
unix:VERSION = 1.0.1
macx:QMAKE_LFLAGS_SONAME = -Wl,-install_name,@executable_path/../Frameworks/

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
