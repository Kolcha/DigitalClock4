#!/bin/bash

export QT_ROOT="$HOME/Qt/5.6/gcc_64"

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
cores=`cat /proc/cpuinfo | grep "processor" | wc -l`
make -j$cores
[[ $? == 0 ]] || exit 1

# create targer folder
TARGET_APP_FOLDER="$CLOCK_SRC_PATH/../$CLOCK_APP_NAME"
rm -rf "$TARGET_APP_FOLDER"
mkdir "$TARGET_APP_FOLDER"
[[ $? == 0 ]] || exit 1

# copy all app files to target folder
cp digital_clock/digital_clock "$TARGET_APP_FOLDER/"
cp clock_common/libclock_common.so.1.1.1 "$TARGET_APP_FOLDER/libclock_common.so.1"
cp plugin_core/libplugin_core.so.1.0.0 "$TARGET_APP_FOLDER/libplugin_core.so.1"
cp skin_draw/libskin_draw.so.1.2.3 "$TARGET_APP_FOLDER/libskin_draw.so.1"
[ -d "$TARGET_APP_FOLDER/plugins" ] || mkdir "$TARGET_APP_FOLDER/plugins"
cd plugins
for i in *
do
  if [ -d "$i" ] && [ -f "$i/lib$i.so" ]
  then
    cp "$i/lib$i.so" "$TARGET_APP_FOLDER/plugins/"
  fi
done
cd ..

# deploy qt runtime
cd "$TARGET_APP_FOLDER"

"$CLOCK_SRC_PATH/scripts/linux_deploy_qt.sh" "digital_clock"
"$CLOCK_SRC_PATH/scripts/linux_deploy_qt.sh" "libclock_common.so.1"
"$CLOCK_SRC_PATH/scripts/linux_deploy_qt.sh" "libplugin_core.so.1"
"$CLOCK_SRC_PATH/scripts/linux_deploy_qt.sh" "libskin_draw.so.1"

for i in $(ls -1 plugins)
do
  if [[ -f "plugins/$i" ]]
  then
    "$CLOCK_SRC_PATH/scripts/linux_deploy_qt.sh" "plugins/$i"
  fi
done

# generate qt.conf
rm -f qt.conf
echo "[Paths]"                     >> qt.conf
echo "Plugins = plugins"           >> qt.conf
echo "Translations = translations" >> qt.conf

# copy resources and some specific stuff
cp "$CLOCK_SRC_PATH/digital_clock/resources/digital_clock.desktop" "$TARGET_APP_FOLDER/"
cp "$CLOCK_SRC_PATH/digital_clock/resources/images/time-24.svg" "$TARGET_APP_FOLDER/digital_clock.svg"
cp "$CLOCK_SRC_PATH/scripts/digital_clock.sh" "$TARGET_APP_FOLDER/"
cp "$CLOCK_SRC_PATH/scripts/config_desktop.sh" "$TARGET_APP_FOLDER/"
cp -r "$CLOCK_DATA_PATH/skins" "$TARGET_APP_FOLDER/"
cp -r "$CLOCK_DATA_PATH/textures" "$TARGET_APP_FOLDER/"
[[ -d "$CLOCK_DATA_PATH/translations" ]] && cp -r "$CLOCK_DATA_PATH/translations" "$TARGET_APP_FOLDER/"

cd "$TARGET_APP_FOLDER/.."

# cleanup
rm -rf "$build_dir"

# create archive
rm -f digital_clock_4-linux.tar.xz
tar cJvf digital_clock_4-linux.tar.xz "$CLOCK_APP_NAME"
[[ $? == 0 ]] || exit 1

# create source tarball
cd "$CLOCK_SRC_PATH/.."
cp -r "$CLOCK_SRC_PATH" digital_clock_4-src
cd digital_clock_4-src
hg clean --all
rm -rf .hg*
cd ..
rm -f digital_clock_4-src.tar.xz
tar cJvf digital_clock_4-src.tar.xz digital_clock_4-src
[[ $? == 0 ]] || exit 1
rm -rf digital_clock_4-src
