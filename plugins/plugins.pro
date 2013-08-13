TEMPLATE = subdirs

SUBDIRS += \
    spectrum_clock \
    var_translucency \
    hour_signal \
    alarm_timer

unix:QMAKE_CXXFLAGS += -std=c++11
