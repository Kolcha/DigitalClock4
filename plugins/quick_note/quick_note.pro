#-------------------------------------------------
#
# Project created by QtCreator 2013-12-29T23:04:10
#
#-------------------------------------------------

QT       += core gui widgets

include(../common.pri)

TARGET = quick_note

SOURCES += quick_note.cpp \
    gui/message_widget.cpp

HEADERS += quick_note.h \
    quick_note_settings.h \
    gui/message_widget.h

TRANSLATIONS += \
    quick_note_ru.ts

include(../../qm_gen.pri)

RESOURCES += quick_note.qrc

OTHER_FILES += quick_note.json

win32:RC_FILE = quick_note.rc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../skin_draw/release/ -lskin_draw
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../skin_draw/debug/ -lskin_draw
else:unix: LIBS += -L$$OUT_PWD/../../skin_draw/ -lskin_draw

INCLUDEPATH += $$PWD/../../skin_draw
DEPENDPATH += $$PWD/../../skin_draw

FORMS += \
    gui/message_widget.ui
