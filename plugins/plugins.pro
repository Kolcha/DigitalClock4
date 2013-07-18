TEMPLATE = subdirs

SUBDIRS += \
    spectrum_clock \
    var_translucency

unix:QMAKE_CXXFLAGS += -std=c++11
