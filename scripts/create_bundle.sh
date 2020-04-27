#!/bin/bash

[[ -n $QT_ROOT ]] || QT_ROOT="/usr/local/Qt-5.12.8"

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

build_dir="$CLOCK_SRC_PATH/../build"
rm -rf "$build_dir"
mkdir "$build_dir"
cd "$build_dir"

$QT_ROOT/bin/qmake $CUSTOM_QMAKE_FLAGS -config release -r "$CLOCK_SRC_PATH/DigitalClock.pro"
make -j$(sysctl -n hw.ncpu)
[[ $? == 0 ]] || exit 1

# create directories
mkdir -p digital_clock/digital_clock.app/Contents/Frameworks
mkdir -p digital_clock/digital_clock.app/Contents/PlugIns

# deploy Qt for libs
cp clock_common/libclock_common.1.2.1.dylib digital_clock/digital_clock.app/Contents/Frameworks/libclock_common.1.dylib
cp plugin_core/libplugin_core.1.3.2.dylib digital_clock/digital_clock.app/Contents/Frameworks/libplugin_core.1.dylib
cp skin_draw/libskin_draw.1.5.1.dylib digital_clock/digital_clock.app/Contents/Frameworks/libskin_draw.1.dylib
$QT_ROOT/bin/macdeployqt digital_clock/digital_clock.app -executable=digital_clock/digital_clock.app/Contents/Frameworks/libclock_common.1.dylib
$QT_ROOT/bin/macdeployqt digital_clock/digital_clock.app -executable=digital_clock/digital_clock.app/Contents/Frameworks/libplugin_core.1.dylib
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
cp -r "$QT_ROOT/plugins/texttospeech" "digital_clock/digital_clock.app/Contents/PlugIns/"

# copy custom Qt plugins
cp paletteicon/libpaletteicon.dylib digital_clock/digital_clock.app/Contents/PlugIns/iconengines/

# deploy copied Qt plugins
for d in iconengines playlistformats texttospeech
do
  rm -rf digital_clock/digital_clock.app/Contents/PlugIns/$d/*.dylib.dSYM
  rm -f digital_clock/digital_clock.app/Contents/PlugIns/$d/*_debug.dylib
  for l in $(ls -1 digital_clock/digital_clock.app/Contents/PlugIns/$d/)
  do
    deploy_qt digital_clock/digital_clock.app/Contents/PlugIns/$d/$l
  done
done

# deploy Qt translations
tr_path="$PWD/digital_clock/digital_clock.app/Contents/Resources/translations"
[[ -d "$tr_path" ]] || mkdir "$tr_path"
pushd "$QT_ROOT/translations" > /dev/null
langs=$(ls -1 qt_*.qm | grep -v help | sed 's/qt_\(.*\)\.qm/\1/g')
for lang in $langs
do
  lang_files=$(ls -1 qt*_$lang.qm)
  $QT_ROOT/bin/lconvert -o "$tr_path/qt_$lang.qm" $lang_files
done
popd > /dev/null

# copy resources
cp -r $CLOCK_DATA_PATH/skins digital_clock/digital_clock.app/Contents/Resources/
cp -r $CLOCK_DATA_PATH/textures digital_clock/digital_clock.app/Contents/Resources/

# update generated qt.conf
echo "Translations = Resources/translations" >> digital_clock/digital_clock.app/Contents/Resources/qt.conf

# deploy Qt for app and create .dmg file
cd digital_clock
mv digital_clock.app "$CLOCK_APP_NAME.app"
$QT_ROOT/bin/macdeployqt "$CLOCK_APP_NAME.app"
find "$CLOCK_APP_NAME.app" -name "*_debug.dylib" -exec rm -vf {} \;
codesign --deep --force --verify --verbose --sign "-" "$CLOCK_APP_NAME.app"
dmgbuild -s "$CLOCK_SRC_PATH/installer/macos/settings.py" -D app="$CLOCK_APP_NAME.app" "$CLOCK_APP_NAME" "$build_dir/../digital_clock_4-macosx.dmg"

# cleanup
cd "$build_dir/.."
rm -rf "$build_dir"
