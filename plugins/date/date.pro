#-------------------------------------------------
#
# Project created by QtCreator 2014-01-01T09:46:16
#
#-------------------------------------------------

QT       += core gui widgets

include(../common.pri)

TARGET = date

SOURCES += \
    date_plugin.cpp \
    gui/settings_dialog.cpp

HEADERS += \
    date_plugin.h \
    date_settings.h \
    gui/settings_dialog.h

FORMS += \
    gui/settings_dialog.ui

TRANSLATIONS += \
    date_ru.ts

include(../../qm_gen.pri)

RESOURCES += date.qrc

OTHER_FILES += date.json

win32:RC_FILE = date.rc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../plugin_core/release/ -lplugin_core
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../plugin_core/debug/ -lplugin_core
else:unix: LIBS += -L$$OUT_PWD/../../plugin_core/ -lplugin_core

INCLUDEPATH += $$PWD/../../plugin_core
DEPENDPATH += $$PWD/../../plugin_core
