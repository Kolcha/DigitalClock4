TEMPLATE = subdirs

SUBDIRS += \
    spectrum_clock

unix:QMAKE_CXXFLAGS += -std=c++11
