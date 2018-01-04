#!/bin/bash

# plugins lists
# core gui widgets
gui_lst="generic imageformats platforminputcontexts platforms platformthemes xcbglintegrations"
# network
net_lst="bearer"
# multimedia
mm_lst="audio mediaservice playlistformats"
# svg
svg_lst="iconengines"
# tts
tts_lst="texttospeech"

# don't look for system Qt!
export LD_LIBRARY_PATH="$QT_ROOT/lib"

src_binary=$1
[[ -z $2 ]] && target_path="." || target_path=$2

function deploy_plugin() {
  qt_libs=$(ldd "$QT_ROOT/$1" | grep "$QT_ROOT" | grep -o -e '\/.*\.so\.[0-9]\+')
  for f in $qt_libs
  do
    [[ -f $(basename "$f") ]] || cp -L "$f" "$target_path/"
  done
}


function deploy_plugins_list() {
  for i in $1
  do
    [[ -d "$target_path/plugins/$i" ]] || cp -r "$QT_ROOT/plugins/$i" "$target_path/plugins/"
    for plg in $(ls -1 "$target_path/plugins/$i/")
    do
      deploy_plugin "plugins/$i/$plg"
    done
  done
}


[[ -d "$target_path/plugins" ]] || mkdir "$target_path/plugins"

qt_libs=$(ldd "$src_binary" | grep "$QT_ROOT" | grep -o -e '\/.*\.so\.[0-9]\+')
for f in $qt_libs
do
  [[ -f $(basename "$f") ]] || cp -L "$f" "$target_path/"
  [[ $(echo "$f" | grep -i "gui") != "" ]] && deploy_plugins_list "$gui_lst"
  [[ $(echo "$f" | grep -i "network") != "" ]] && deploy_plugins_list "$net_lst"
  [[ $(echo "$f" | grep -i "multimedia") != "" ]] && deploy_plugins_list "$mm_lst"
  [[ $(echo "$f" | grep -i "svg") != "" ]] && deploy_plugins_list "$svg_lst"
  [[ $(echo "$f" | grep -i "texttospeech") != "" ]] && deploy_plugins_list "$tts_lst"
done

if ! [[ -d "$target_path/translations" ]]
then
  mkdir "$target_path/translations"
  tr_path=$(readlink -f "$target_path/translations")
  pushd "$QT_ROOT/translations" > /dev/null
  langs=$(ls -1 qt_*.qm | grep -v help | sed 's/qt_\(.*\)\.qm/\1/g')
  for lang in $langs
  do
    lang_files=$(ls -1 qt*_$lang.qm)
    "$QT_ROOT/bin/lconvert" -o "$tr_path/qt_$lang.qm" $lang_files
  done
  popd > /dev/null
fi

qt_core_path="$target_path/libQt5Core.so.5"
offset=$(grep -a -b -o 'qt_prfxpath=' "$qt_core_path" | grep -o '[0-9]\+')
offset=$((offset + 12))
path_len=${#target_path}
printf $target_path | dd of="$qt_core_path" bs=1 count=$path_len seek=$offset conv=notrunc 2> /dev/null
zero_bytes=$((260-path_len))
zero_start=$((offset+path_len))
dd if=/dev/zero of="$qt_core_path" bs=1 count=$zero_bytes seek=$zero_start conv=notrunc 2> /dev/null
