#-------------------------------------------------
#
# Project created by QtCreator 2013-12-29T23:04:10
#
#-------------------------------------------------

QT       += core gui widgets

include(../common.pri)

TARGET = quick_note

SOURCES += \
    quick_note_plugin.cpp \
    gui/message_widget.cpp \
    gui/settings_dialog.cpp

HEADERS += \
    quick_note_plugin.h \
    quick_note_settings.h \
    gui/message_widget.h \
    gui/settings_dialog.h

FORMS += \
    gui/message_widget.ui \
    gui/settings_dialog.ui

TRANSLATIONS += \
    quick_note_ru.ts

include(../../qm_gen.pri)

RESOURCES += quick_note.qrc

OTHER_FILES += quick_note.json

win32:RC_FILE = quick_note.rc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../plugin_core/release/ -lplugin_core
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../plugin_core/debug/ -lplugin_core
else:unix: LIBS += -L$$OUT_PWD/../../plugin_core/ -lplugin_core

INCLUDEPATH += $$PWD/../../plugin_core
DEPENDPATH += $$PWD/../../plugin_core
