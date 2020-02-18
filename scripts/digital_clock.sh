#!/bin/bash
#
# File:   digital_clock.sh
# Author: Nick Korotysh <nick.korotysh@gmail.com>
#
# Copyright (C) 2014-2020 Nick Korotysh
#
# Created on November 9, 2014, 11:36 AM
#
# Clock launcher script
#

function running_on_kde()
{
  [[ "$XDG_CURRENT_DESKTOP" == "KDE" || "$XDG_SESSION_DESKTOP" == "KDE" ]]
  return $?
}


function compare_versions()
{
  lhs=($(echo $1 | tr '.' '\n'))
  rhs=($(echo $2 | tr '.' '\n'))
  res=0
  for ((i=0; i<3; i++))
  do
    [[ $res -ne 0 ]] && break
    [[ ${lhs[i]} -gt ${rhs[i]} ]] && { res=1; }
    [[ ${lhs[i]} -lt ${rhs[i]} ]] && { res=-1; }
  done
  echo $res
}


function has_suitable_system_qt()
{
  required_vers=$(echo $1 | sed 's/\..$/\.0/g')
  qt_core=$(find /usr/lib* -name libQt5Core.so.5 2> /dev/null)
  [[ -e "$qt_core" ]] || return $?
  version=$(ls -1 $qt_core* | grep -Po '\d+\.\d+\.\d+')
  [[ $(compare_versions $version $required_vers) -ge 0 ]]
  return $?
}


# handle script arguments
is_autostart=0

app_args=()
while [[ $# -gt 0 ]]
do
  key="$1"
  case $key in
    --autostart)
      is_autostart=1
      shift
      ;;

    *)
      app_args+=("$1")
      shift
      ;;
  esac
done
set -- "${app_args[@]}"     # set everything unhandled back to $@

[[ $is_autostart -ne 0 ]] && sleep 15

this_script=$(readlink -ne "$0")
script_path=$(dirname "$this_script")

export LD_LIBRARY_PATH=$script_path:$LD_LIBRARY_PATH

local_qt_core="$script_path/qt/libQt5Core.so.5"
if [[ -x "$local_qt_core" ]]
then
  cd "$script_path"         # work in application directory
  qt_vers=$("$local_qt_core" | grep -Po 'Qt \d+\.\d+\.\d+' | cut -c 3- -)
  # on KDE systems try to use system Qt libraries instead of shipped ones
  if !(running_on_kde) || !(has_suitable_system_qt "$qt_vers")
  then
    export QT_QPA_PLATFORMTHEME=gtk2
    export LD_LIBRARY_PATH="$script_path/qt:$LD_LIBRARY_PATH"
  fi
fi

"${this_script/%.sh}" "$@"
