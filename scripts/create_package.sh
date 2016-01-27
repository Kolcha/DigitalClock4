#!/bin/bash

QT_PATH="$HOME/Qt/5.5/gcc_64/bin"

CLOCK_APP_NAME="Digital Clock 4"

CLOCK_SRC_PATH="$HOME/Documents/Qt/digital_clock_4"
CLOCK_DATA_PATH="$HOME/Documents/Qt/digital_clock_data"

# some validation
failed=0
[ -d "$QT_PATH" ] || { failed=1; }
[[ $failed == 1 ]] && echo "QT_PATH=$QT_PATH doesn't exists"
[ -d "$CLOCK_SRC_PATH" ] || { failed=1; }
[[ $failed == 1 ]] && echo "CLOCK_SRC_PATH=$CLOCK_SRC_PATH doesn't exists"
[ -d "$CLOCK_DATA_PATH" ] || { failed=1; }
[[ $failed == 1 ]] && echo "CLOCK_DATA_PATH=$CLOCK_DATA_PATH doesn't exists"
[[ $failed == 1 ]] && exit 1

cd "$CLOCK_SRC_PATH"

$QT_PATH/lupdate -no-obsolete DigitalClock.pro
$QT_PATH/qmake -config release
make

# create targer folder
TARGET_APP_FOLDER="$CLOCK_SRC_PATH/../$CLOCK_APP_NAME"
rm -rf "$TARGET_APP_FOLDER"
mkdir "$TARGET_APP_FOLDER"

# copy Qt runtime files
cp -r "$CLOCK_DATA_PATH/qt_linux/"* "$TARGET_APP_FOLDER/"

# copy all app files to target folder
cp digital_clock/digital_clock "$TARGET_APP_FOLDER/"
cp clock_common/libclock_common.so.1.0.8 "$TARGET_APP_FOLDER/libclock_common.so.1"
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

# copy resources and some specific stuff
cp digital_clock/resources/digital_clock.desktop "$TARGET_APP_FOLDER/"
cp digital_clock/resources/images/time-24.svg "$TARGET_APP_FOLDER/digital_clock.svg"
cp scripts/digital_clock.sh "$TARGET_APP_FOLDER/"
cp scripts/config_desktop.sh "$TARGET_APP_FOLDER/"
cp -r "$CLOCK_DATA_PATH/skins" "$TARGET_APP_FOLDER/"
cp -r "$CLOCK_DATA_PATH/textures" "$TARGET_APP_FOLDER/"

# cleanup
make distclean

# create archive
cd "$TARGET_APP_FOLDER/.."
rm -f digital_clock_4-linux.tar.xz
tar cJvf digital_clock_4-linux.tar.xz "$CLOCK_APP_NAME"

# create source tarball
cp -r "$CLOCK_SRC_PATH" digital_clock_4-src
cd digital_clock_4-src
hg clean --all
rm -rf .hg*
cd ..
rm -f digital_clock_4-src.tar.xz
tar cJvf digital_clock_4-src.tar.xz digital_clock_4-src
rm -rf digital_clock_4-src
