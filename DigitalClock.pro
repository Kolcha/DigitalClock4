TEMPLATE = subdirs

SUBDIRS += \
    skin_draw \
    digital_clock \
    plugins

unix:QMAKE_CXXFLAGS += -std=c++11
