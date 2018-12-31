#!/bin/bash
#
# File:   config_desktop.sh
# Author: Nick Korotysh <nick.korotysh@gmail.com>
#
# Copyright (C) 2014-2019 Nick Korotysh
#
# Created on November 7, 2014, 10:36 PM
#
# Script to configure .desktop file on target system
#

cd "${0%/*}"

DESKTOP_FILE="digital_clock.desktop"
APP_EXEC="\"$PWD/digital_clock.sh\""
APP_ICON="$PWD/digital_clock.svg"

if [ -z "$1" ]
then
  # change .desktop file
  APP_EXEC_S=$(echo $APP_EXEC | sed 's/\//\\\//g')
  APP_ICON_S=$(echo $APP_ICON | sed 's/\//\\\//g')

  sed -i "s/^Exec=.*$/Exec=$APP_EXEC_S/g" $DESKTOP_FILE
  sed -i "s/^Icon=.*$/Icon=$APP_ICON_S/g" $DESKTOP_FILE

  chmod 755 $DESKTOP_FILE

  if [[ $UID -ne 0 ]]
  then
    LOCAL_APPS_DIR="$HOME/.local/share/applications"
  else
    LOCAL_APPS_DIR="/usr/local/share/applications"
  fi
  [ -d $LOCAL_APPS_DIR ] || mkdir -p $LOCAL_APPS_DIR
  ln -sf "$PWD/$DESKTOP_FILE" "$LOCAL_APPS_DIR/$DESKTOP_FILE"
fi

# parse script options
if [[ "$1" == "--help" ]]
then
  echo "Supported options:"
  echo "  --icon-color <arg> : change application icon color"
  echo "      <arg> value : hex HTML color without '#'"
  exit
fi

if [[ "$1" == "--icon-color" ]]
then
  color=$(echo "$2" | grep -e "[A-Fa-f0-9]\{6\}")
  if [ -z "$color" ]
  then
    echo "invalid argument"
    exit
  fi
  sed -i "s/fill=\"\#[A-Fa-f0-9]\{6\}\"/fill=\"\#$color\"/g" "$APP_ICON"
  exit
fi
