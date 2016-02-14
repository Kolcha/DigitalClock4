#!/bin/bash

QT_ROOT="$HOME/Qt5.6.0/5.6/clang_64"

CLOCK_APP_NAME="Digital Clock 4"

CLOCK_SRC_PATH="$HOME/Documents/Qt/DigitalClock4"
CLOCK_DATA_PATH="$CLOCK_SRC_PATH/../data"

# helper function to change Qt libraries paths for any library/executable
function deploy_qt() {
  qt_libs=$(otool -L "$1" | grep "$QT_ROOT" | sed 's/ .*//g')

  for l in $qt_libs
  do
    c_name=$(basename $l)
    install_name_tool -change $l @rpath/$c_name.framework/Versions/5/$c_name "$1"
  done
}


cd $CLOCK_SRC_PATH

$QT_ROOT/bin/lupdate -no-obsolete DigitalClock.pro
$QT_ROOT/bin/qmake QMAKE_MAC_SDK=macosx10.9 -config release
make

# create directories
mkdir -p digital_clock/digital_clock.app/Contents/Frameworks
mkdir -p digital_clock/digital_clock.app/Contents/PlugIns

# deploy Qt for libs
cp clock_common/libclock_common.1.1.0.dylib digital_clock/digital_clock.app/Contents/Frameworks/libclock_common.1.dylib
cp skin_draw/libskin_draw.1.2.3.dylib digital_clock/digital_clock.app/Contents/Frameworks/libskin_draw.1.dylib
$QT_ROOT/bin/macdeployqt digital_clock/digital_clock.app -executable=digital_clock/digital_clock.app/Contents/Frameworks/libclock_common.1.dylib
$QT_ROOT/bin/macdeployqt digital_clock/digital_clock.app -executable=digital_clock/digital_clock.app/Contents/Frameworks/libskin_draw.1.dylib

# deploy Qt for clock plugins
for item in $(ls -1 plugins)
do
  if [[ -d "plugins/$item" ]] && [[ -f "plugins/$item/lib$item.dylib" ]]
  then
    cp "plugins/$item/lib$item.dylib" "digital_clock/digital_clock.app/Contents/PlugIns/"
    $QT_ROOT/bin/macdeployqt digital_clock/digital_clock.app -executable=digital_clock/digital_clock.app/Contents/PlugIns/lib$item.dylib
  fi
done

# copy some Qt plugins
cp -r "$QT_ROOT/plugins/iconengines" "digital_clock/digital_clock.app/Contents/PlugIns/"
cp -r "$QT_ROOT/plugins/playlistformats" "digital_clock/digital_clock.app/Contents/PlugIns/"

# deploy Qt plugins
for d in iconengines playlistformats
do
  for l in $(ls -1 digital_clock/digital_clock.app/Contents/PlugIns/$d/)
  do
    deploy_qt digital_clock/digital_clock.app/Contents/PlugIns/$d/$l
  done
done

# copy resources
cp -r $CLOCK_DATA_PATH/skins digital_clock/digital_clock.app/Contents/Resources
cp -r $CLOCK_DATA_PATH/textures digital_clock/digital_clock.app/Contents/Resources
cp -r $CLOCK_DATA_PATH/translations digital_clock/digital_clock.app/Contents/
cp -f $CLOCK_DATA_PATH/qt.conf digital_clock/digital_clock.app/Contents/Resources/

# deploy Qt for app and create .dmg file
cd digital_clock
rm -f *.dmg
rm -rf "$CLOCK_APP_NAME.app"
mv digital_clock.app "$CLOCK_APP_NAME.app"
$QT_ROOT/bin/macdeployqt "$CLOCK_APP_NAME.app" -dmg
mv "$CLOCK_APP_NAME.dmg" digital_clock_4-macosx.dmg
cd ..

# cleanup
make distclean