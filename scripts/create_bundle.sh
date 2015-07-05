#!/bin/bash

QT_PATH="$HOME/Qt/5.5/clang_64/bin"

CLOCK_APP_NAME="Digital Clock 4"

CLOCK_SRC_PATH="$HOME/Documents/Qt/DigitalClock4"
CLOCK_DATA_PATH="$CLOCK_SRC_PATH/../data"

cd $CLOCK_SRC_PATH

$QT_PATH/lupdate -no-obsolete DigitalClock.pro
$QT_PATH/qmake -config release
make

# deploy Qt for app
$QT_PATH/macdeployqt digital_clock/digital_clock.app

# deploy Qt for libs
cp clock_common/libclock_common.1.0.7.dylib digital_clock/digital_clock.app/Contents/Frameworks/libclock_common.1.dylib
cp skin_draw/libskin_draw.1.2.2.dylib digital_clock/digital_clock.app/Contents/Frameworks/libskin_draw.1.dylib
$QT_PATH/macdeployqt digital_clock/digital_clock.app -executable=digital_clock/digital_clock.app/Contents/Frameworks/libclock_common.1.dylib
$QT_PATH/macdeployqt digital_clock/digital_clock.app -executable=digital_clock/digital_clock.app/Contents/Frameworks/libskin_draw.1.dylib

# deploy Qt for clock plugins
oldIFS="$IFS"
IFS='
'

items=($(ls -1A plugins))
for item in "${items[@]}"
do
  if [[ -d "plugins/$item" ]] && [[ -f "plugins/$item/lib$item.dylib" ]]
  then
    cp "plugins/$item/lib$item.dylib" "digital_clock/digital_clock.app/Contents/PlugIns/"
    $QT_PATH/macdeployqt digital_clock/digital_clock.app -executable=digital_clock/digital_clock.app/Contents/PlugIns/lib$item.dylib
  fi
done

IFS="$oldIFS"

# copy resources
cp -r $CLOCK_DATA_PATH/skins digital_clock/digital_clock.app/Contents/
cp -r $CLOCK_DATA_PATH/textures digital_clock/digital_clock.app/Contents/
cp -r $CLOCK_DATA_PATH/translations digital_clock/digital_clock.app/Contents/
cp -f $CLOCK_DATA_PATH/qt.conf digital_clock/digital_clock.app/Contents/Resources/

# create .dmg file
cd digital_clock
rm -f *.dmg
mv digital_clock.app "$CLOCK_APP_NAME.app"
$QT_PATH/macdeployqt "$CLOCK_APP_NAME.app" -dmg
mv "$CLOCK_APP_NAME.dmg" digital_clock_4-macosx.dmg
cd ..

# cleanup
make clean
find . -name "*.dylib" -exec rm -f {} \;
find . -name "Makefile" -exec rm -f {} \;
rm -rf "digital_clock/$CLOCK_APP_NAME.app"
rm -f .qmake.stash
