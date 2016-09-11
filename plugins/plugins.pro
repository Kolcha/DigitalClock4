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
    date \
    move_clock \
    tower_clock_signal \
    ip_address \
    quarter_hour_signal \
    custom_signal

windows {
SUBDIRS += \
    power_off

win32-msvc* {
SUBDIRS += \
    win_on_top
}
}
