TEMPLATE = subdirs

SUBDIRS += \
    spectrum_clock \
    var_translucency \
    hour_signal \
    alarm \
    tray_color \
    any_zoom \
    schedule \
    quick_note \
    date

windows {
SUBDIRS += power_off
}
