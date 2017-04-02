CONFIG += C++11

HEADERS += $$PWD/QHotkey/qhotkey.h \
	$$PWD/QHotkey/qhotkey_p.h

SOURCES += $$PWD/QHotkey/qhotkey.cpp

mac {
	SOURCES += $$PWD/QHotkey/qhotkey_mac.cpp
	LIBS += -framework Carbon
} else:win32 {
	SOURCES += $$PWD/QHotkey/qhotkey_win.cpp
	LIBS += -lUser32
} else:unix {
	SOURCES += $$PWD/QHotkey/qhotkey_x11.cpp
	QT += x11extras
	LIBS += -lX11
}

INCLUDEPATH += $$PWD/QHotkey

DISTFILES += \
    $$PWD/QHotkey/qhotkey.pri
