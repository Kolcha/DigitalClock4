TEMPLATE = subdirs

SUBDIRS += \
    spectrum_clock \
    var_translucency \
    hour_signal \
    alarm

unix:QMAKE_CXXFLAGS += -std=c++11
