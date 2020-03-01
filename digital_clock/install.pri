#
#   Digital Clock - beautiful customizable clock with plugins
#   Copyright (C) 2020  Nick Korotysh <nick.korotysh@gmail.com>
#
#   This program is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

unix:!macx {
    target.path = $$clock_libs_path
    INSTALLS += target

    launcher_script.files = $$PWD/../scripts/digital_clock.sh
    launcher_script.path = $$clock_libs_path
    launcher_script.CONFIG += nostrip
    INSTALLS += launcher_script

    launcher_symlink.path = $$prefix/bin
    launcher_symlink.extra = $$QMAKE_SYMBOLIC_LINK \
            `realpath -m --relative-to=\$(INSTALL_ROOT)$$launcher_symlink.path \$(INSTALL_ROOT)$$clock_libs_path/digital_clock.sh` \
            \$(INSTALL_ROOT)$$prefix/bin/digitalclock4
    INSTALLS += launcher_symlink

    desktop_file.files = resources/digital_clock.desktop
    desktop_file.path = $$prefix/share/applications
    INSTALLS += desktop_file

    app_icon.target = digitalclock4.svg
    app_icon.commands = cp $$PWD/resources/images/if_time-24_103169.svg $$OUT_PWD/$$app_icon.target
    app_icon.files = $$OUT_PWD/$$app_icon.target
    app_icon.path = $$prefix/share/icons/hicolor/scalable/apps
    app_icon.CONFIG += no_check_exist
    INSTALLS += app_icon
}
