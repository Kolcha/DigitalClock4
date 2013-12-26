TEMPLATE = subdirs

SUBDIRS += \
    spectrum_clock \
    var_translucency \
    hour_signal \
    alarm \
    tray_color \
    any_zoom \
    schedule

windows {
SUBDIRS += power_off
}
