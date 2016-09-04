#-------------------------------------------------
#
# Project created by QtCreator 2016-05-27T13:13:13
#
#-------------------------------------------------

QT       += core gui widgets network

include(../common.pri)

TARGET = ip_address

SOURCES += \
    ip_address_plugin.cpp \
    gui/settings_dialog.cpp

HEADERS += \
    ip_address_plugin.h \
    gui/settings_dialog.h

FORMS += \
    gui/settings_dialog.ui

TRANSLATIONS += \
    ip_address_ru.ts

include(../../qm_gen.pri)

RESOURCES += ip_address.qrc

OTHER_FILES += ip_address.json

win32:RC_FILE = ip_address.rc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../plugin_core/release/ -lplugin_core
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../plugin_core/debug/ -lplugin_core
else:unix: LIBS += -L$$OUT_PWD/../../plugin_core/ -lplugin_core

INCLUDEPATH += $$PWD/../../plugin_core
DEPENDPATH += $$PWD/../../plugin_core
