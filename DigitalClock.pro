TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS += \
    clock_common \
    skin_draw \
    digital_clock \
    plugin_core \
    plugins

digital_clock.depends = clock_common
digital_clock.depends = skin_draw
plugins.depends = plugin_core
