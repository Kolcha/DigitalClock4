TEMPLATE = subdirs

SUBDIRS += \
    clock_common \
    skin_draw \
    digital_clock \
    plugins

unix:QMAKE_CXXFLAGS += -std=c++11
