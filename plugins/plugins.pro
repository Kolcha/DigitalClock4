TEMPLATE = subdirs

SUBDIRS += \
    spectrum_clock \
    var_translucency \
    hour_signal \
    alarm \
    am_pm

unix:QMAKE_CXXFLAGS += -std=c++11
