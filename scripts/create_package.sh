#!/bin/bash

[[ -n $QT_ROOT ]] || export QT_ROOT="/usr/local/Qt-5.12.3"

CLOCK_APP_NAME="Digital Clock 4"

CLOCK_SRC_PATH="$HOME/Documents/Qt/digital_clock_4"
CLOCK_DATA_PATH="$HOME/Documents/Qt/digital_clock_data"

# some validation
failed=0
[ -d "$QT_ROOT" ] || { failed=1; }
[[ $failed == 1 ]] && echo "QT_ROOT=$QT_ROOT doesn't exists"
[ -d "$CLOCK_SRC_PATH" ] || { failed=1; }
[[ $failed == 1 ]] && echo "CLOCK_SRC_PATH=$CLOCK_SRC_PATH doesn't exists"
[ -d "$CLOCK_DATA_PATH" ] || { failed=1; }
[[ $failed == 1 ]] && echo "CLOCK_DATA_PATH=$CLOCK_DATA_PATH doesn't exists"
[[ $failed == 1 ]] && exit 1

cd "$CLOCK_SRC_PATH"

$QT_ROOT/bin/lupdate -no-obsolete DigitalClock.pro

build_dir="$CLOCK_SRC_PATH/../build"
rm -rf "$build_dir"
mkdir "$build_dir"
cd "$build_dir"

$QT_ROOT/bin/qmake -config release -r "$CLOCK_SRC_PATH/DigitalClock.pro"
cores=$(grep processor < /proc/cpuinfo | wc -l)
make -j$cores
[[ $? == 0 ]] || exit 1

# create targer folder
TARGET_APP_FOLDER="$CLOCK_SRC_PATH/../$CLOCK_APP_NAME"
rm -rf "$TARGET_APP_FOLDER"
mkdir "$TARGET_APP_FOLDER"
[[ $? == 0 ]] || exit 1

# copy all app files to target folder
cp digital_clock/digital_clock "$TARGET_APP_FOLDER/"
cp clock_common/libclock_common.so.1.2.1 "$TARGET_APP_FOLDER/libclock_common.so.1"
cp plugin_core/libplugin_core.so.1.3.1 "$TARGET_APP_FOLDER/libplugin_core.so.1"
cp skin_draw/libskin_draw.so.1.5.1 "$TARGET_APP_FOLDER/libskin_draw.so.1"

# strip binaries
strip -s "$TARGET_APP_FOLDER/digital_clock"
strip -s "$TARGET_APP_FOLDER/libclock_common.so.1"
strip -s "$TARGET_APP_FOLDER/libplugin_core.so.1"
strip -s "$TARGET_APP_FOLDER/libskin_draw.so.1"

[ -d "$TARGET_APP_FOLDER/plugins" ] || mkdir "$TARGET_APP_FOLDER/plugins"
cd plugins
for i in *
do
  if [ -d "$i" ] && [ -f "$i/lib$i.so" ]
  then
    cp "$i/lib$i.so" "$TARGET_APP_FOLDER/plugins/"
    strip -s "$TARGET_APP_FOLDER/plugins/lib$i.so"
  fi
done
cd ..

# deploy qt runtime
cd "$TARGET_APP_FOLDER"
qt_rt="./qt"
mkdir "$qt_rt"
"$CLOCK_SRC_PATH/scripts/linux_deploy_qt.sh" "digital_clock" "$qt_rt"
"$CLOCK_SRC_PATH/scripts/linux_deploy_qt.sh" "libclock_common.so.1" "$qt_rt"
"$CLOCK_SRC_PATH/scripts/linux_deploy_qt.sh" "libplugin_core.so.1" "$qt_rt"
"$CLOCK_SRC_PATH/scripts/linux_deploy_qt.sh" "libskin_draw.so.1" "$qt_rt"

for i in $(ls -1 plugins)
do
  if [[ -f "plugins/$i" ]]
  then
    "$CLOCK_SRC_PATH/scripts/linux_deploy_qt.sh" "plugins/$i" "$qt_rt"
  fi
done

# copy custom Qt plugins
mkdir "$TARGET_APP_FOLDER/iconengines"
cp "$build_dir/paletteicon/libpaletteicon.so" "$TARGET_APP_FOLDER/iconengines/"
strip -s "$TARGET_APP_FOLDER/iconengines/libpaletteicon.so"

# copy SSL libs
cp "/usr/lib/x86_64-linux-gnu/libssl.so.1.1" "$TARGET_APP_FOLDER/$qt_rt/"
cp "/usr/lib/x86_64-linux-gnu/libcrypto.so.1.1" "$TARGET_APP_FOLDER/$qt_rt/"

# remove unused styles
for i in $(ls -1 "$qt_rt/plugins/styles" | grep -v gtk)
do
  rm "$qt_rt/plugins/styles/$i"
done

# copy resources and some specific stuff
cp "$CLOCK_SRC_PATH/digital_clock/resources/digital_clock.desktop" "$TARGET_APP_FOLDER/"
cp "$CLOCK_SRC_PATH/digital_clock/resources/images/time-24.svg" "$TARGET_APP_FOLDER/digital_clock.svg"
cp "$CLOCK_SRC_PATH/scripts/digital_clock.sh" "$TARGET_APP_FOLDER/"
cp "$CLOCK_SRC_PATH/scripts/config_desktop.sh" "$TARGET_APP_FOLDER/"
cp -r "$CLOCK_DATA_PATH/skins" "$TARGET_APP_FOLDER/"
cp -r "$CLOCK_DATA_PATH/textures" "$TARGET_APP_FOLDER/"
[[ -f "$CLOCK_DATA_PATH/install_linux.rtf" ]] && cp "$CLOCK_DATA_PATH/install_linux.rtf" "$TARGET_APP_FOLDER/INSTALL.rtf"
cp "$CLOCK_SRC_PATH/installer/postinstall_readme.txt" "$TARGET_APP_FOLDER/README.txt"

cd "$TARGET_APP_FOLDER/.."

# cleanup
rm -rf "$build_dir"

# create archive
rm -f digital_clock_4-linux.tar.xz
tar cJvf digital_clock_4-linux.tar.xz "$CLOCK_APP_NAME"
[[ $? == 0 ]] || exit 1
