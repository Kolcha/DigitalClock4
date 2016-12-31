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


function deploy_plugin() {
  qt_libs=$(ldd "$QT_ROOT/$1" | grep "$QT_ROOT" | grep -o -e '\/.*\.so\.[0-9]\+')
  for f in $qt_libs
  do
    [[ -f $(basename "$f") ]] || cp -L "$f" ./
  done
}


function deploy_plugins_list() {
  for i in $1
  do
    [[ -d "plugins/$i" ]] || cp -r "$QT_ROOT/plugins/$i" plugins/
    for plg in $(ls -1 "plugins/$i/")
    do
      deploy_plugin "plugins/$i/$plg"
    done
  done
}


[[ -d plugins ]] || mkdir plugins

qt_libs=$(ldd "$1" | grep "$QT_ROOT" | grep -o -e '\/.*\.so\.[0-9]\+')
for f in $qt_libs
do
  [[ -f $(basename "$f") ]] || cp -L "$f" ./
  [[ $(echo "$f" | grep -i "gui") != "" ]] && deploy_plugins_list "$gui_lst"
  [[ $(echo "$f" | grep -i "network") != "" ]] && deploy_plugins_list "$net_lst"
  [[ $(echo "$f" | grep -i "multimedia") != "" ]] && deploy_plugins_list "$mm_lst"
  [[ $(echo "$f" | grep -i "svg") != "" ]] && deploy_plugins_list "$svg_lst"
  [[ $(echo "$f" | grep -i "texttospeech") != "" ]] && deploy_plugins_list "$tts_lst"
done
